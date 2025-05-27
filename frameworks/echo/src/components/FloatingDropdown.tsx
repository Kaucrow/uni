import React, { useState, useRef, useEffect } from 'react';
import './FloatingDropdown.css';

type DropdownOption = {
  id: string;
  label: string;
  icon?: React.ReactNode;
  onClick: () => void;
};

interface FloatingDropdownProps {
  triggerContent: React.ReactNode;
  options: DropdownOption[];
  position?: 'top-left' | 'top-right' | 'bottom-left' | 'bottom-right';
}

const FloatingDropdown: React.FC<FloatingDropdownProps> = ({
  triggerContent,
  options,
  position = 'bottom-right',
}) => {
  const [isOpen, setIsOpen] = useState(false);
  const [isSticky, setIsSticky] = useState(false);
  const [expandsUp, setExpandsUp] = useState(false);
  const dropdownRef = useRef<HTMLDivElement>(null);
  const triggerRef = useRef<HTMLButtonElement>(null);

  const getNavbarHeight = (): number => {
    const navbarHeight = getComputedStyle(document.documentElement)
      .getPropertyValue('--navbar-height');

      return parseInt(navbarHeight, 10) || 70;
  }
  
  let navbarHeight = getNavbarHeight();
  const [offset, setOffset] = useState(navbarHeight);

  useEffect(() => {
    const handleScroll = () => {
      console.log('here');
      if (triggerRef.current) {
        const triggerRect = triggerRef.current.getBoundingClientRect();
        const scrollY = window.scrollY || window.pageYOffset;
        const absoluteTop = triggerRect.top + scrollY;
      
        console.log(`IS STICKY: ${isSticky}`);
        setIsSticky(scrollY > navbarHeight);
        setOffset(navbarHeight - scrollY);
      }
    };

    window.addEventListener('scroll', handleScroll);
    return () => window.removeEventListener('scroll', handleScroll);
  }, []);

  // Calculate if we should expand up or down
  useEffect(() => {
    if (isOpen && triggerRef.current) {
      const triggerRect = triggerRef.current.getBoundingClientRect();
      const spaceBelow = window.innerHeight - triggerRect.bottom;
      const dropdownHeight = options.length * 48 + 16; // Approximate height (48px per item + padding)
      
      setExpandsUp(spaceBelow < dropdownHeight);
    }
  }, [isOpen, options.length]);

  // Close dropdown when clicking outside
  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      if (dropdownRef.current && !dropdownRef.current.contains(event.target as Node)) {
        setIsOpen(false);
      }
    };

    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  return (
    <div
      className={`floating-dropdown ${position}`}
      ref={dropdownRef}
      style={{
        top: isSticky ? '0px' : offset,
      }}
    >
      <button 
        ref={triggerRef}
        className="dropdown-trigger"
        onClick={() => setIsOpen(!isOpen)}
        aria-expanded={isOpen}
        aria-haspopup="true"
      >
        {triggerContent}
      </button>

      {isOpen && (
        <div className={`dropdown-menu ${expandsUp ? 'expands-up' : 'expands-down'}`}>
          {options.map((option) => (
            <button
              key={option.id}
              className="dropdown-item"
              onClick={() => {
                option.onClick();
                setIsOpen(false);
              }}
            >
              {option.icon && <span className="item-icon">{option.icon}</span>}
              {option.label}
            </button>
          ))}
        </div>
      )}
    </div>
  );
};

export default FloatingDropdown;