# Loom Video Demo Script (7-10 mins)

## 1. Intro & Architecture (1 min)
- **Visual**: Show the Architecture Diagram (from README).
- **Script**: "Hello! This is a demo of an end-to-end IoT system. We have three main components:
    1. An **ESP32** simulated in Wokwi, sending sensor data.
    2. A **NestJS Backend** that processes and stores this data in SQLite.
    3. A **Flutter App** that visualizes the health metrics in real-time."

## 2. Backend API Demo (2 mins)
- **Visual**: VS Code (NestJS project), Terminal, Postman/Browser.
- **Action**:
    - Show `readings.controller.ts` and `readings.entity.ts`.
    - Run the backend: `npm run start`.
    - Open browser/Postman to `http://localhost:3000/readings/latest`.
    - Show it returning `null` or empty initially.
- **Script**: "Here is our NestJS backend. It uses TypeORM and SQLite. I'll start the server... and as you see, the database is currently empty/waiting."

## 3. ESP32 Wokwi Demo (3 mins)
- **Visual**: Wokwi Browser Tab.
- **Action**:
    - Show `sketch.ino`. Explain the ADC to Temperature conversion logic.
    - Start Simulation.
    - Show Serial Monitor output: `Connecting... Connected... POST 201`.
    - Move the Potentiometer knob.
- **Script**: "Now let's simulate the hardware. I'm using Wokwi. This code reads the Potentiometer, calculates a temperature between 35-42°C, and sends it to our local backend. Watch the Serial Monitor as I move the knob..."

## 4. Flutter App Demo (3 mins)
- **Visual**: iOS Simulator / Android Emulator running the App.
- **Action**:
    - Run the app: `flutter run`.
    - Show the "Loading" state.
    - Once data flows, show the cards updating every 3 seconds.
    - Change Wokwi Potentiometer again, wait 3s, see Flutter update.
- **Script**: "Finally, here is the Flutter app. It polls the API every 3 seconds. You can see the Temperature, Heart Rate, and Raw ADC value. Let me change the sensor value one last time... and there it updates on the phone!"

## 5. Conclusion (1 min)
- **Visual**: Split screen (Wokwi + Emulator).
- **Script**: "That's the full loop! Sensor to Cloud to Mobile. The code is structured, modular, and ready for production expansion. Thanks for watching!"
