import React from 'react';
import { LucideIcon } from 'lucide-react';

interface SensorCardProps {
  title: string;
  value: number | string;
  unit: string;
  Icon: LucideIcon;
  color: 'rose' | 'sky' | 'emerald' | 'violet';
}

const colorVariants = {
  rose: 'bg-rose-50 border-rose-200 text-rose-600',
  sky: 'bg-sky-50 border-sky-200 text-sky-600',
  emerald: 'bg-emerald-50 border-emerald-200 text-emerald-600',
  violet: 'bg-violet-50 border-violet-200 text-violet-600',
};

export function SensorCard({ title, value, unit, Icon, color }: SensorCardProps) {
  return (
    <div className={`rounded-xl border-2 p-6 transition-transform hover:scale-105 ${colorVariants[color]}`}>
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold">{title}</h3>
        <Icon className="w-6 h-6" />
      </div>
      <div className="flex items-baseline">
        <span className="text-3xl font-bold">{value}</span>
        <span className="ml-2 opacity-75">{unit}</span>
      </div>
    </div>
  );
}