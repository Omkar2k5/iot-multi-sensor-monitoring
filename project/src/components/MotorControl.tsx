import React from 'react';
import { Power } from 'lucide-react';
import { useMotorControl } from '../hooks/useMotorControl';

export function MotorControl() {
  const { motorStatus, toggleMotor } = useMotorControl();

  return (
    <div className="bg-white/50 backdrop-blur-sm rounded-xl border-2 border-indigo-200 p-6">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-indigo-900">Water Motor Control</h3>
        <Power className="text-indigo-600" size={24} />
      </div>
      <button
        onClick={toggleMotor}
        className={`w-full py-3 px-4 rounded-lg font-medium transition-all transform hover:scale-105 ${
          motorStatus
            ? 'bg-emerald-500 hover:bg-emerald-600 text-white'
            : 'bg-rose-500 hover:bg-rose-600 text-white'
        }`}
      >
        {motorStatus ? 'Turn Off Motor' : 'Turn On Motor'}
      </button>
    </div>
  );
}