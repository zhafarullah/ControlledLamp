# Controlling lamp with hand gesture (ON/OFF)
## Introduction
This project aims to turn lights on and off using hand gestures. It combines the principles of image processing and recognition with IoT technology. Image processing and recognition are used to detect hand gestures, while IoT, based on the MQTT protocol, is employed to transmit the detection results to a microcontroller, specifically the ESP32, which controls the lights.

## Demo Video : https://youtu.be/2zyslDbxnYM
## Required components
1. ESP32/Arduino Uno
2. AC Lamp
3. Lamp Fitting
4. Relay
5. Computer with camera

## Steps to Set Up

### 1. Circuit Assembly
- Connect the **COM** pin of the relay to the **V+** of the power source.
- Connect the **NO** (Normally Open) pin of the relay to the **V+** of the lamp.
- Connect the ground directly from the power source to the lamp.

---

### 2. Mosquitto Broker Setup
- Ensure a Mosquitto broker is running on your system.
- Add the broker's IP address to the `main.cpp` program.
- see here for details of [MQTT protocol on IoT](https://github.com/zhafarullah/MQTTIoT)

---

### 3. ESP32 Code Deployment
- Navigate to the `src` folder.
- Build and upload the `main.cpp` file to your ESP32 microcontroller.

---

### 4. Testing Script
- Add the broker's IP address to the `testing.py` script.
- Run `testing.py` on your system.
- Ensure the required model is created. You can follow the [model creation tutorial](https://github.com/zhafarullah/HandPoseRecognition) for guidance.

---

### 5. Verify Communication
- Ensure the topic is successfully transmitted to the broker and received by the ESP32.
- Check the terminal output of `testing.py` and the serial monitor of the ESP32 to confirm communication.

---

## Notes
- If you encounter any issues during setup, verify the circuit connections and ensure the broker is running correctly.
- Ensure you have installed all necessary dependencies for `testing.py` and the ESP32 code.
