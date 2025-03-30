import { useState, useEffect } from 'react';
import { ref, onValue } from 'firebase/database';
import { db } from '../services/firebase';

export function useMotorControl() {
  const [moisture1, setMoisture1] = useState(0);
  const [moisture2, setMoisture2] = useState(0);
  const [motorActive, setMotorActive] = useState(false);
  const MOISTURE_THRESHOLD = 40;

  useEffect(() => {
    const currentRef = ref(db, 'current');
    
    const unsubscribe = onValue(currentRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        // Handle case sensitivity and ensure numeric values
        const m1 = Number(data.moistureLevel || data.moisturelevel || 0);
        const m2 = Number(data.moistureLevel2 || data.moisturelevel2 || 0);
        
        setMoisture1(m1);
        setMoisture2(m2);

        // Set motor status based solely on moisture levels
        const shouldMotorBeActive = m1 < MOISTURE_THRESHOLD || m2 < MOISTURE_THRESHOLD;
        setMotorActive(shouldMotorBeActive);

        console.log('System status:', {
          moisture1: m1,
          moisture2: m2,
          moistureThreshold: MOISTURE_THRESHOLD,
          motorActive: shouldMotorBeActive
        });
      }
    });

    return () => {
      unsubscribe();
    };
  }, []);

  const toggleMotor = async () => {
    alert('Motor is automatically controlled based on moisture levels');
    return;
  };

  return {
    motorStatus: motorActive,
    toggleMotor,
    moisture1,
    moisture2,
    isLowMoisture: moisture1 < MOISTURE_THRESHOLD || moisture2 < MOISTURE_THRESHOLD
  };
}