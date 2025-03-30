import { FC } from 'react';
import './SignalIndicator.css';

interface SignalIndicatorProps {
  isMotorOn: boolean;
}

const SignalIndicator: FC<SignalIndicatorProps> = ({ isMotorOn }) => {
  return (
    <div className="signal-indicator">
      <div className={`indicator-light ${isMotorOn ? 'active' : ''}`} />
      <span>Motor Status: {isMotorOn ? 'ON' : 'OFF'}</span>
    </div>
  );
};

export default SignalIndicator; 