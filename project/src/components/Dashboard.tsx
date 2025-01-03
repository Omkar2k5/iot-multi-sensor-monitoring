import React from 'react';
import { Thermometer, Droplets, Waves, Sprout } from 'lucide-react';
import { SensorCard } from './SensorCard';
import { SensorGraph } from './SensorGraph';
import { MotorControl } from './MotorControl';
import { useSensorData } from '../hooks/useSensorData';

export function Dashboard() {
  const { currentData, historicalData, isLoading } = useSensorData();

  if (isLoading || !currentData) {
    return (
      <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 flex items-center justify-center">
        <p className="text-xl font-semibold text-indigo-600">Loading sensor data...</p>
      </div>
    );
  }

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
            title="Moisture Level"
            value={currentData.moistureLevel}
            unit="%"
            Icon={Sprout}
            color="violet"
          />
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mb-8">
          <SensorGraph
            title="Temperature"
            value={currentData.temperature}
            unit="°C"
            Icon={Thermometer}
            color="rose"
            data={historicalData.map(d => ({ value: d.temperature, timestamp: d.timestamp }))}
          />
          <SensorGraph
            title="Humidity"
            value={currentData.humidity}
            unit="%"
            Icon={Droplets}
            color="sky"
            data={historicalData.map(d => ({ value: d.humidity, timestamp: d.timestamp }))}
          />
          <SensorGraph
            title="Water Level"
            value={currentData.waterLevel}
            unit="%"
            Icon={Waves}
            color="emerald"
            data={historicalData.map(d => ({ value: d.waterLevel, timestamp: d.timestamp }))}
          />
          <SensorGraph
            title="Moisture Level"
            value={currentData.moistureLevel}
            unit="%"
            Icon={Sprout}
            color="violet"
            data={historicalData.map(d => ({ value: d.moistureLevel, timestamp: d.timestamp }))}
          />
        </div>

        <div className="mt-8">
          <MotorControl />
        </div>
      </div>
    </div>
  );
}