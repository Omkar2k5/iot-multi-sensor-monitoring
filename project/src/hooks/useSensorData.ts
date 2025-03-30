import { useEffect, useState } from 'react';
import { ref, onValue, off } from 'firebase/database';
import { db } from '../services/firebase';
import { SensorData, parseSensorData } from '../utils/sensorDataParser';

export function useSensorData() {
  const [currentData, setCurrentData] = useState<SensorData | null>(null);
  const [historicalData, setHistoricalData] = useState<SensorData[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const MOISTURE_THRESHOLD = 40;

  useEffect(() => {
    const currentRef = ref(db, 'current');
    
    const unsubscribe = onValue(currentRef, (snapshot) => {
      if (snapshot.exists()) {
        const data = snapshot.val();
        // Get moisture values and check threshold
        const moisture1 = Number(data.moistureLevel) || 0;
        const moisture2 = Number(data.moisturelevel2) || 0;
        const shouldMotorBeOn = moisture1 < MOISTURE_THRESHOLD || moisture2 < MOISTURE_THRESHOLD;

        const parsedData = {
          ...parseSensorData(data),
          motor: { status: shouldMotorBeOn ? 'ON' : 'OFF' },
          timestamp: Date.now() // Ensure we have a current timestamp
        };

        setCurrentData(parsedData);
        
        // Update historical data with the new reading
        setHistoricalData(prev => {
          const newData = [...prev, parsedData];
          // Keep only the last 20 readings
          return newData.slice(-20);
        });
      }
      setIsLoading(false);
    });

    return () => {
      unsubscribe();
      off(currentRef);
    };
  }, []);

  return { 
    currentData, 
    historicalData: historicalData.map(data => ({
      ...data,
      timestamp: new Date(data.timestamp)
    })), 
    isLoading 
  };
}