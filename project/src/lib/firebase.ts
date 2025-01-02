import { initializeApp } from 'firebase/app';
import { getDatabase, ref, set, push } from 'firebase/database';

// Firebase configuration
const firebaseConfig = {
  apiKey: import.meta.env.VITE_FIREBASE_API_KEY,
  authDomain: import.meta.env.VITE_FIREBASE_AUTH_DOMAIN,
  databaseURL: import.meta.env.VITE_FIREBASE_DATABASE_URL,
  projectId: import.meta.env.VITE_FIREBASE_PROJECT_ID,
  storageBucket: import.meta.env.VITE_FIREBASE_STORAGE_BUCKET,
  messagingSenderId: import.meta.env.VITE_FIREBASE_MESSAGING_SENDER_ID,
  appId: import.meta.env.VITE_FIREBASE_APP_ID,
};

// Initialize Firebase app
const app = initializeApp(firebaseConfig);

// Get database instance
export const db = getDatabase(app);

// Define the type for sensor data
interface SensorData {
  humidity: number;
  moistureLevel: string;
  temperature: string;
  waterLevel: string;
  timestamp: string; // ISO format string
}

// Function to update current data and push to history
export const updateCurrentData = (data: SensorData) => {
  const currentRef = ref(db, 'current');
  const historyRef = ref(db, 'history');

  // 1. Update the current data
  set(currentRef, data);

  // 2. Add the data to the history section with a new unique key
  const newHistoryRef = push(historyRef);
  set(newHistoryRef, {
    ...data,
    timestamp: new Date().toISOString(), // Ensure we have the current timestamp
  });
};
