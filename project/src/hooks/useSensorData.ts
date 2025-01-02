import { useEffect, useState } from 'react';
import { ref, onValue } from 'firebase/database';
import { db } from '../lib/firebase';
import { ParsedSensorData } from '../types/sensor';
import { parseSensorData } from '../utils/sensorDataParser';

const MAX_DATA_POINTS = 10;

export function useSensorData() {
  const [currentData, setCurrentData] = useState<ParsedSensorData | null>(null);
  const [historicalData, setHistoricalData] = useState<ParsedSensorData[]>([]);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const currentRef = ref(db, '/current');
    
    const unsubscribe = onValue(currentRef, (snapshot) => {
      const rawData = snapshot.val();
      const parsedData = parseSensorData(rawData);
      
      if (parsedData) {
        setCurrentData(parsedData);
        setHistoricalData(prev => {
          const newData = [...prev, parsedData];
          return newData.slice(-MAX_DATA_POINTS);
        });
      }
      setIsLoading(false);
    });

    return () => unsubscribe();
  }, []);

  return { currentData, historicalData, isLoading };
}