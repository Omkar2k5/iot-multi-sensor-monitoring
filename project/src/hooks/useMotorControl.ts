import { useEffect, useState } from 'react';
import { ref, onValue, set } from 'firebase/database';
import { db } from '../lib/firebase';

export function useMotorControl() {
  const [motorStatus, setMotorStatus] = useState(false);

  useEffect(() => {
    const motorRef = ref(db, '/motor/status');
    const unsubscribe = onValue(motorRef, (snapshot) => {
      const status = snapshot.val();
      setMotorStatus(status === "ON");
    });

    return () => unsubscribe();
  }, []);

  const toggleMotor = async () => {
    try {
      const newStatus = !motorStatus;
      await set(ref(db, '/motor/status'), newStatus ? "ON" : "OFF");
    } catch (error) {
      console.error('Error toggling motor:', error);
    }
  };

  return { motorStatus, toggleMotor };
}