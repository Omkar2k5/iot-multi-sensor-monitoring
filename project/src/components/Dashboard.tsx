import React, { FC } from 'react';
import { Thermometer, Droplets, Waves, Sprout } from 'lucide-react';
import { SensorCard } from './SensorCard';
import { useSensorData } from '../hooks/useSensorData';
import SignalIndicator from './SignalIndicator';

export const Dashboard: FC = () => {
  const { currentData, isLoading } = useSensorData();
  const MOISTURE_THRESHOLD = 40;

  if (isLoading || !currentData) {
    return (
      <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 flex items-center justify-center">
        <p className="text-xl font-semibold text-indigo-600">Loading sensor data...</p>
      </div>
    );
  }

  // Calculate motor status based on moisture levels
  const isMotorOn = currentData.moistureLevel < MOISTURE_THRESHOLD || 
                    currentData.moistureLevel2 < MOISTURE_THRESHOLD;

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 p-6">
      <div className="max-w-7xl mx-auto">
        <h1 className="text-3xl font-bold text-indigo-900 mb-8">IoT Monitoring System</h1>
        
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 mb-8">
          <SensorCard
            title="Temperature"
            value={currentData.temperature}
            unit="°C"
            Icon={Thermometer}
            color="rose"
          />
          <SensorCard
            title="Humidity"
            value={currentData.humidity}
            unit="%"
            Icon={Droplets}
            color="sky"
          />
          <SensorCard
            title="Water Level"
            value={currentData.waterLevel}
            unit="%"
            Icon={Waves}
            color="emerald"
          />
          <SensorCard
            title="Moisture Level 1 "
            value={currentData.moistureLevel}
            unit="%"
            Icon={Sprout}
            color="violet"
          />
          <SensorCard
            title="Moisture Level 2"
            value={currentData.moistureLevel2}
            unit="%"
            Icon={Sprout}
            color="violet"
          />
        </div>

        <SignalIndicator 
          isMotorOn={isMotorOn}
        />
      </div>
    </div>
  );
};