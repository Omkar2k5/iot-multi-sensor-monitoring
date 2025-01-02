import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer } from 'recharts';
import { format } from 'date-fns';

interface DataPoint {
  timestamp: number;
  value: number;
}

interface SensorChartProps {
  data: DataPoint[];
  dataKey: string;
  color: string;
  unit: string;
}

export function SensorChart({ data, dataKey, color, unit }: SensorChartProps) {
  const formattedData = data.map(point => ({
    ...point,
    time: format(point.timestamp, 'HH:mm:ss'),
  }));

  return (
    <div className="h-64 w-full">
      <ResponsiveContainer width="100%" height="100%">
        <LineChart data={formattedData} margin={{ top: 5, right: 30, left: 20, bottom: 5 }}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="time" />
          <YAxis unit={unit} />
          <Tooltip
            formatter={(value: number) => [`${value}${unit}`, dataKey]}
            labelFormatter={(label) => `Time: ${label}`}
          />
          <Line
            type="monotone"
            dataKey="value"
            stroke={color}
            strokeWidth={2}
            dot={false}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}