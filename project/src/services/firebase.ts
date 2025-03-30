import { initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';

const firebaseConfig = {
  apiKey: "AIzaSyCCUFV-XhN4yUODnncdOiFFGi1Ozp99vI4",
  authDomain: "iot-monitoring-system-d1df9.firebaseapp.com",
  databaseURL: "https://iot-monitoring-system-d1df9-default-rtdb.firebaseio.com",
  projectId: "iot-monitoring-system-d1df9",
  storageBucket: "iot-monitoring-system-d1df9.appspot.com",
  messagingSenderId: "your-messaging-sender-id",
  appId: "your-app-id"
};

const app = initializeApp(firebaseConfig);
export const db = getDatabase(app); 