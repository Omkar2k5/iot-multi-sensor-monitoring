export interface SensorData {
  temperature: number;
  humidity: number;
  waterLevel: number;
  moistureLevel: number;
  motorStatus: number;
  timestamp: number;
}

export function parseSensorData(data: any): SensorData {
  return {
    temperature: Number(data.temperature) || 0,
    humidity: Number(data.humidity) || 0,
    waterLevel: Number(data.waterLevel) || 0,
    moistureLevel: Number(data.moistureLevel) || 0,
    motorStatus: Number(data.motorStatus) || 0,
    timestamp: Number(data.timestamp) || Date.now()
  };
} 