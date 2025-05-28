import { useEffect } from 'react';

export const useDarkMode = () => {
  useEffect(() => {
    const toggle = document.getElementById('toggle') as HTMLInputElement | null;
    const html = document.documentElement;

    if (!toggle) return;

    // Check for saved preference or system preference
    const savedTheme = localStorage.getItem('theme');
    const systemPrefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;

    // Set initial theme
    if (savedTheme === 'dark' || (!savedTheme && systemPrefersDark)) {
      html.classList.add('dark');
      toggle.checked = true;
    }

    const handleToggleChange = () => {
      if (toggle.checked) {
        html.classList.add('dark');
        localStorage.setItem('theme', 'dark');
      } else {
        html.classList.remove('dark');
        localStorage.setItem('theme', 'light');
      }
    };

    const handleSystemThemeChange = (e: MediaQueryListEvent) => {
      if (!localStorage.getItem('theme')) {
        if (e.matches) {
          html.classList.add('dark');
          toggle.checked = true;
        } else {
          html.classList.remove('dark');
          toggle.checked = false;
        }
      }
    };

    toggle.addEventListener('change', handleToggleChange);
    const colorSchemeQuery = window.matchMedia('(prefers-color-scheme: dark)');
    colorSchemeQuery.addEventListener('change', handleSystemThemeChange);

    return () => {
      toggle.removeEventListener('change', handleToggleChange);
      colorSchemeQuery.removeEventListener('change', handleSystemThemeChange);
    };
  }, []);
};