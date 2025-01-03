import cv2
import mediapipe as mp
import numpy as np
from tensorflow.keras.models import load_model
import paho.mqtt.client as mqtt

broker_address = "20.2.168.202" #Broker IP address
broker_port = 1883  #Broker Port
topic = "hand_pose/data"

mqtt_client = mqtt.Client()
mqtt_client.connect(broker_address, broker_port)

model = load_model("hand_pose_dnn_model.h5") #model Name

label_map = {0: "fist (OFF)", 1: "hifive(ON)", 2: "none(Nothing)"} 

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(static_image_mode=False,
                       max_num_hands=1,
                       min_detection_confidence=0.5,
                       min_tracking_confidence=0.5)
mp_drawing = mp.solutions.drawing_utils

def extract_landmarks(results):
    if results.multi_hand_landmarks:
        hand_landmarks = results.multi_hand_landmarks[0]
        landmarks = []
        for lm in hand_landmarks.landmark:
            landmarks.extend([lm.x, lm.y, lm.z])
        return np.array(landmarks).reshape(1, -1)  
    return None

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

print("Press 'q' to quit.")

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame.")
        break

    frame = cv2.flip(frame, 1)

    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(rgb_frame)

    label_text = "none(Nothing)"  

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

        landmarks = extract_landmarks(results)
        if landmarks is not None:
            prediction = model.predict(landmarks)
            predicted_label = np.argmax(prediction)
            prediction_confidence = np.max(prediction) * 100

            if prediction_confidence < 70:  
                label_text = "none(Nothing)"
            else:
                label_text = label_map[predicted_label]

    mqtt_client.publish(topic, label_text)
    print(f"Data sent to MQTT: {label_text}")

    cv2.putText(frame, label_text, (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    cv2.imshow('Hand Pose Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
mqtt_client.disconnect()
