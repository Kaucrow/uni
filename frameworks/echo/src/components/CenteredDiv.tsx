import type { ReactNode } from 'react';
import './CenteredDiv.css';

interface CenteredDivProps {
  children: ReactNode;
  className?: string;
  backgroundColor?: string;
  height?: string;
  padding?: string;
  paddingTop?: string;
}

const CenteredDiv: React.FC<CenteredDivProps> = ({ 
  children,
  className = '',
  backgroundColor = 'transparent',
  height = '100%',
  padding = '0px',
  paddingTop = '0px',
}) => {
  return (
    <div 
      className='outer-container'
      style={{ 
        '--bg-color': backgroundColor,
        '--height': height,
        '--padding': padding,
        '--padding-top': paddingTop,
      } as React.CSSProperties}
    >
      <div className={`inner-container ${className}`}>
        {children}
      </div>
    </div>
  );
};

export default CenteredDiv;