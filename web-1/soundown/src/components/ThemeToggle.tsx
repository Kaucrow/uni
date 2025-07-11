import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faMoon, faSun } from '@fortawesome/free-solid-svg-icons';
import { useEffect, useState } from 'react';
import { Button } from './ui/button';

const ThemeToggle = () => {
  // Initialize theme based on localStorage or default to 'light'
  const [theme, setTheme] = useState(() => {
    if (typeof window !== 'undefined') {
      return localStorage.getItem('theme') || 'light';
    }
    return 'light';
  });

  useEffect(() => {
    const root = window.document.documentElement; // This is the <html> element
    root.classList.remove('light', 'dark'); // Remove existing themes
    root.classList.add(theme); // Add current theme

    // Save theme preference to localStorage
    localStorage.setItem('theme', theme);
  }, [theme]); // Re-run effect when theme changes

  const toggleTheme = () => {
    setTheme((prevTheme) => (prevTheme === 'light' ? 'dark' : 'light'));
  };

  return (
    <Button
      onClick={toggleTheme}
      className="
        min-w-10
        p-2 rounded-md
        transition-colors duration-200 ease-in-out
        flex items-center justify-center
        focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2 dark:focus:ring-offset-neutral-800
      "
    >
      {theme === 'light' ? (
        <FontAwesomeIcon icon={faMoon} />
      ) : (
        <FontAwesomeIcon icon={faSun} />
      )}
    </Button>
  );
};

export default ThemeToggle;