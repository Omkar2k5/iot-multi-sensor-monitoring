export interface SensorData {
  temperature: string;
  humidity: string;
  waterLevel: string;
  moistureLevel: string;
  timestamp: string;
}

export interface ParsedSensorData {
  temperature: number;
  humidity: number;
  waterLevel: number;
  moistureLevel: number;
  timestamp: Date;
}