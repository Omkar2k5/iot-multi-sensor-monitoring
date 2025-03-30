import React, { FC } from 'react';
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
  violet: '#7c3aed',
  amber: '#d97706'
};

export const SensorGraph: FC<SensorGraphProps> = ({ title, value, unit, Icon, color, data }) => {
  const formatDate = (timestamp: Date) => {
    return timestamp.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  };

  return (
    <div className="bg-white rounded-lg shadow-md p-6">
      <div className="flex items-center justify-between mb-4">
        <div className="flex items-center gap-2">
          <Icon className={`text-${color}-500`} />
          <h3 className="text-lg font-semibold">{title}</h3>
        </div>
        <div className="text-2xl font-bold">
          {value}
          <span className="text-sm text-gray-500 ml-1">{unit}</span>
        </div>
      </div>
      <ResponsiveContainer width="100%" height={200}>
        <LineChart data={data}>
          <XAxis 
            dataKey="timestamp" 
            tickFormatter={formatDate}
            interval="preserveStartEnd"
            stroke="#64748b"
          />
          <YAxis 
            stroke="#64748b"
            domain={['auto', 'auto']}
          />
          <Tooltip 
            labelFormatter={(label) => formatDate(new Date(label))}
            formatter={(value) => [`${value}${unit}`, title]}
          />
          <Line
            type="monotone"
            dataKey="value"
            stroke={colorMap[color as keyof typeof colorMap]}
            strokeWidth={2}
            dot={false}
            isAnimationActive={true}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
};