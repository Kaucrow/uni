import type { ReactNode } from 'react';
import './CenteredDiv.css';

interface CenteredDivProps {
  children: ReactNode;
  className?: string;
  maxWidth?: string;
  backgroundColor?: string;
  padding?: string;
}

const CenteredDiv: React.FC<CenteredDivProps> = ({ 
  children,
  className = '',
  maxWidth = '800px',
  backgroundColor = 'transparent',
  padding = '20px'
}) => {
  return (
    <div 
      className='outer-container'
      style={{ 
        '--max-width': maxWidth,
        '--bg-color': backgroundColor,
        '--padding': padding
      } as React.CSSProperties}
    >
      <div className={`inner-container ${className}`}>
        {children}
      </div>
    </div>
  );
};

export default CenteredDiv;