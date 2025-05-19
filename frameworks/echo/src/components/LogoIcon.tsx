interface LogoIconProps {
  className?: string;
  color?: string;
  width?: number;
  height?: number;
}

const LogoIcon: React.FC<LogoIconProps> = ({
  className = '',
  color = "#f8d56b",
  width = 45,
  height =
  45
}) => {
  return (
    <svg 
      className={`logo-icon ${className}`}
      width={width} 
      height={height} 
      viewBox="0 0 200 200" 
      xmlns="http://www.w3.org/2000/svg"
    >
      {/* Thick outer ring */}
      <circle cx="100" cy="100" r="80" stroke={color} strokeWidth="14" fill="none"/>

      {/* Diagonal cross */}
      <line 
        x1="45" 
        y1="45" 
        x2="155" 
        y2="155" 
        stroke={color} 
        strokeWidth="14" 
        strokeLinecap="round"
      />
      <line 
        x1="155" 
        y1="45" 
        x2="45" 
        y2="155" 
        stroke={color} 
        strokeWidth="14" 
        strokeLinecap="round"
      />

      {/* Inner glow effect */}
      <circle 
        cx="100" 
        cy="100" 
        r="40" 
        fill="none" 
        stroke={color} 
        strokeWidth="2" 
        opacity="0.7"
      >
        <animate 
          attributeName="r" 
          values="40;45;40" 
          dur="4s" 
          repeatCount="indefinite"
        />
      </circle>
    </svg>
  );
};

export default LogoIcon;