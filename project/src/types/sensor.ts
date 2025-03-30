export interface SensorData {
  temperature: number;
  humidity: number;
  waterLevel: number;
  moistureLevel: number;
  moistureLevel2: number;
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

export interface ParsedSensorData {
  temperature: number;
  humidity: number;
  waterLevel: number;
  moistureLevel: number;
  moistureLevel2: number;
  timestamp: Date;
}