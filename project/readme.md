# 🌍 **IoT Multi-Sensor Monitoring System**

An IoT-based web application designed to monitor environmental factors such as **temperature**, **humidity**, **water level**, and **soil moisture**. The system also includes an **on/off switch** to control a water motor, providing a comprehensive solution for real-time monitoring and control.

---

## 🚀 **Features**

- **Real-Time Sensor Monitoring**:
  - **Temperature**: Displays real-time temperature readings.
  - **Humidity**: Tracks and logs ambient humidity.
  - **Water Level**: Monitors and visualizes water level data.
  - **Soil Moisture**: Assesses soil moisture levels for agricultural purposes.
  
- **Motor Control**: Enables users to turn the water motor on/off via the application.

- **Data Storage**: Sensor data is stored in **Firebase** for analysis and historical tracking.

- **Responsive Design**: Ensures the application works seamlessly on various devices.

- **Firebase Hosting**: The project is deployed on **Firebase Hosting** for easy accessibility.

---

## 🛠 **Tech Stack**

- **Frontend**: React, TypeScript, Vite
- **Backend**: Firebase (Firestore for database, Hosting for deployment)
- **Hardware**: Arduino with Wi-Fi module for data transmission
- **State Management**: React Hooks
- **Styling**: CSS

---

## 🔑 **Prerequisites**

1. **Node.js** and **npm** installed.
2. **Firebase project** setup:
   - Enable **Firestore Database**.
   - Configure **Firebase Hosting**.
   - Generate **Firebase configuration keys**.
3. **Arduino** with Wi-Fi module (e.g., ESP8266 or ESP32).

---

## ⚙️ **Installation**

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/iot-multi-sensor-monitoring.git
cd iot-multi-sensor-monitoring
```
### 2. Install Dependencies
```
npm install
```
### 3. Set up Firebase Configuration
Create a firebase.ts file inside the src/config directory.
Add your Firebase configuration:
typescript
Copy code
```
export const firebaseConfig = {
  apiKey: "YOUR_API_KEY",
  authDomain: "YOUR_PROJECT_ID.firebaseapp.com",
  projectId: "YOUR_PROJECT_ID",
  storageBucket: "YOUR_PROJECT_ID.appspot.com",
  messagingSenderId: "YOUR_SENDER_ID",
  appId: "YOUR_APP_ID",
};
```
### 4. Run the Development Server
Copy code
```
npm run dev
```
### 🏗 Build and Deployment
1. Build for Production
```
npm run build
```
2. Deploy to Firebase Hosting
#Install Firebase CLI:
```
Copy code
npm install -g firebase-tools
```
#Login to Firebase:
```
firebase login
```
#Initialize Firebase in your project:
```
firebase init
```
#Deploy your app:
```
firebase deploy
```
## 📂 **Folder Structure**

```bash
src/
├── components/      # React components
├── services/        # Firebase operations (e.g., CRUD)
├── config/          # Firebase configuration
├── types/           # TypeScript types for sensors and motor control
├── App.tsx          # Main application
├── main.tsx         # Entry point
├── index.css        # Global styles
```
## 🔧 Sensors Used
#### Temperature Sensor (e.g., DHT11/DHT22): Measures ambient temperature.
#### Humidity Sensor (e.g., DHT11/DHT22): Tracks humidity levels.
#### Water Level Sensor: Monitors water level in storage tanks.
#### Soil Moisture Sensor: Measures moisture levels in the soil for irrigation purposes.
## ⚙️ Usage
#### Arduino Integration
#### Connect the sensors (temperature, humidity, water level, soil moisture) to the Arduino.
#### Configure the Wi-Fi module to send data to Firebase in real-time.
#### Web Application
#### Access the hosted web app on Firebase.
#### Monitor real-time sensor data on the dashboard.
#### Use the motor control switch to manage water flow based on soil moisture and water level.
## ⚠️ Troubleshooting
#### Sensor Data Not Appearing:
Verify the Arduino Wi-Fi module is configured correctly.
Check Firebase database rules and API keys.
#### Build Errors:
Ensure all dependencies are installed and Firebase configuration is correct.
#### Motor Control Issues:
Confirm the motor is connected properly and the control signal is working.
## 🚀 Future Enhancements
Add support for additional sensors (e.g., pH sensors for water quality monitoring).
Implement data analytics for predictive maintenance.
Introduce user authentication for secure access.
###💡 Contributing
Contributions are welcome! Please fork the repository and submit a pull request for review.

## 📝 License
This project is licensed under the MIT License. See the LICENSE file for details.

## 📸 Screenshots
#Web Application Output
![Web App Output](screenshot.png)
#Firebase Realtime Database (RTDB) Screenshot
![Firebase RTDB](firebase-screenshot.png)
