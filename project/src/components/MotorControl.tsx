import React from 'react';
import { Power } from 'lucide-react';
import { useMotorControl } from '../hooks/useMotorControl';

export function MotorControl() {
  const { motorStatus, toggleMotor, moisture1, moisture2, isLowMoisture } = useMotorControl();
  const MOISTURE_THRESHOLD = 40;

  const getStatusMessage = () => {
    if (isLowMoisture) {
      if (moisture1 < MOISTURE_THRESHOLD && moisture2 < MOISTURE_THRESHOLD) {
        return 'Both sensors low - Motor ON';
      } else if (moisture1 < MOISTURE_THRESHOLD) {
        return 'Sensor 1 low - Motor ON';
      } else if (moisture2 < MOISTURE_THRESHOLD) {
        return 'Sensor 2 low - Motor ON';
      }
    }
    return motorStatus ? 'Motor Running' : 'Motor Off';
  };

  return (
    <div className="bg-white/50 backdrop-blur-sm rounded-xl border-2 border-indigo-200 p-6">
      <div className="flex items-center justify-between mb-4">
        <div>
          <h3 className="text-lg font-semibold text-indigo-900">Water Motor Control</h3>
          <p className="text-sm text-gray-600">
            Status: {getStatusMessage()}
          </p>
          <p className="text-xs text-gray-500">
            Moisture 1: {moisture1}% | Moisture 2: {moisture2}%
          </p>
        </div>
        <Power 
          className={`${motorStatus ? 'text-emerald-600' : 'text-rose-600'}`} 
          size={24} 
        />
      </div>
      <button
        onClick={toggleMotor}
        disabled={isLowMoisture}
        className={`w-full py-3 px-4 rounded-lg font-medium transition-all transform hover:scale-105 
          ${motorStatus
            ? 'bg-emerald-500 hover:bg-emerald-600 text-white'
            : 'bg-rose-500 hover:bg-rose-600 text-white'
          }
          ${isLowMoisture ? 'opacity-50 cursor-not-allowed' : ''}`}
      >
        {motorStatus ? 'Turn Off Motor' : 'Turn On Motor'}
      </button>
    </div>
  );
}