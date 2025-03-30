import React from 'react';
import { Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis } from 'recharts';
import { LucideIcon } from 'lucide-react';

interface SensorGraphProps {
  title: string;
  value: number;
  unit: string;
  Icon: LucideIcon;
  color: string;
  data: { value: number; timestamp: Date }[];
}

const colorMap = {
  rose: '#e11d48',
  sky: '#0284c7',
  emerald: '#059669',
  violet: '#7c3aed'
};

export function SensorGraph({ title, value, unit, Icon, color, data }: SensorGraphProps) {
  const chartData = data.map(point => ({
    value: point.value,
    time: point.timestamp.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
  }));

  return (
    <div className="bg-white/50 backdrop-blur-sm rounded-xl border-2 border-gray-200 p-6">
      <div className="flex items-center justify-between mb-4">
        <div className="flex items-center gap-2">
          <Icon className={`w-6 h-6 text-${color}-600`} />
          <h3 className="text-lg font-semibold text-gray-900">{title}</h3>
        </div>
        <div className="text-2xl font-bold text-gray-900">
          {value}
          <span className="ml-1 text-sm font-normal text-gray-500">{unit}</span>
        </div>
      </div>
      
      <div className="h-48">
        <ResponsiveContainer width="100%" height="100%">
          <LineChart data={chartData}>
            <XAxis 
              dataKey="time"
              stroke="#6b7280"
              fontSize={12}
              tickLine={false}
              tickCount={5}
            />
            <YAxis
              stroke="#6b7280"
              fontSize={12}
              tickLine={false}
              unit={unit}
              domain={[0, 'auto']}
            />
            <Tooltip 
              contentStyle={{ 
                backgroundColor: 'rgba(255, 255, 255, 0.8)',
                border: '1px solid #e5e7eb'
              }}
            />
            <Line
              type="monotone"
              dataKey="value"
              stroke={colorMap[color as keyof typeof colorMap]}
              strokeWidth={2}
              dot={false}
              activeDot={{ r: 4 }}
            />
          </LineChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
}