export interface SensorData {
  temperature: number;
  humidity: number;
  waterLevel: number;
  moistureLevel: number;
  moistureLevel2: number;
  motorStatus: number;
  timestamp: number;
  motor?: {
    status: string;
  };
  current?: {
    motorStatus: number;
    humidity: number;
    moistureLevel: number;
    moistureLevel2: number;
    temperature: number;
    timestamp: number;
    waterLevel: number;
  };
}

export function parseSensorData(data: any): SensorData {
  return {
    temperature: Number(data.temperature) || 0,
    humidity: Number(data.humidity) || 0,
    waterLevel: Number(data.waterLevel) || 0,
    moistureLevel: Number(data.moistureLevel) || 0,
    moistureLevel2: Number(data.moisturelevel2) || 0,
    motorStatus: Number(data.motorStatus) || 0,
    timestamp: Number(data.timestamp) || Date.now(),
    ...(data.motor && { motor: data.motor }),
    ...(data.current && { current: data.current })
  };
} 