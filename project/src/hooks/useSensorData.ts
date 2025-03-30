import { useEffect, useState } from 'react';
import { ref, onValue, off } from 'firebase/database';
import { db } from '../services/firebase';
import { SensorData, parseSensorData } from '../utils/sensorDataParser';

export function useSensorData() {
  const [currentData, setCurrentData] = useState<SensorData | null>(null);
  const [historicalData, setHistoricalData] = useState<SensorData[]>([]);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const currentRef = ref(db, 'current');
    const motorRef = ref(db, 'motor');
    
    const unsubscribe = onValue(currentRef, (snapshot) => {
      if (snapshot.exists()) {
        const data = parseSensorData(snapshot.val());
        setCurrentData(prev => ({
          ...prev,
          ...data
        }));
        setHistoricalData(prev => {
          const newData = [...prev, data];
          // Keep last 20 readings
          return newData.slice(-20);
        });
      }
      setIsLoading(false);
    });

    onValue(motorRef, (snapshot) => {
      const motorData = snapshot.val();
      if (motorData) {
        setCurrentData(prev => ({
          ...prev,
          motor: motorData
        }));
      }
    });

    return () => {
      off(currentRef);
      off(motorRef);
    };
  }, []);

  return { currentData, historicalData, isLoading };
}