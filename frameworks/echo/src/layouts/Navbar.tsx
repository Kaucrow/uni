import './Navbar.css';
import { useState } from 'react';
import { useDarkMode } from '../hooks/useDarkMode';
import LogoIcon from '../components/LogoIcon';
import Sidebar from '../components/Sidebar';
import { Link } from 'react-router-dom';

const Navbar = () => {
  useDarkMode();

  let [isSidebarOpen, setIsSidebarOpen] = useState(false);
  function openSidebar() {
    setIsSidebarOpen(true); 
  }
  function closeSidebar() {
    setIsSidebarOpen(false); 
  }

  return (
    <div className='header'>
      <div className='header-container'>
        {/* Menu and logo */}
        <div className='menu-container'>
          {/* <!-- Hamburger --> */ }
          <button onClick={openSidebar} className='hamburger-btn'>
            <svg className='hamburger-icon' viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16m-7 6h7"></path>
            </svg>
            <div className='hamburger-effect'></div>
          </button>

          <Link to='/' className='navbar-logo-link'>
            {/* Logo icon */}
            <LogoIcon />
            {/* Logo text */}
            <p className='navbar-logo-text'>Echoes</p>
          </Link>

          {/* <!-- Large screen navbar --> */}
          <nav className='main-nav'>
            <Link to='/' className='nav-link'>Home</Link>
            <Link to='/auth/login' className='nav-link'>Login</Link>
            <Link to='/auth/register' className='nav-link'>Register</Link>
            <Link to='#!' className='nav-link'>About</Link>
            <Link to='#!' className='nav-link'>Contact</Link>
          </nav>
        </div>

        <div className="header-right">
          {/* <!-- User icon --> */}
          <Link to='/user' className="user-icon">
            <svg xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 448 512">
              <path d="M224 256A128 128 0 1 0 224 0a128 128 0 1 0 0 256zm-45.7 48C79.8 304 0 383.8 0 482.3C0 498.7 13.3 512 29.7 512l388.6 0c16.4 0 29.7-13.3 29.7-29.7C448 383.8 368.2 304 269.7 304l-91.4 0z"/>
            </svg>
          </Link>

          {/* <!-- Dark mode switch --> */}
          <div className="dark-mode-toggle">
            <input type="checkbox" id="toggle" className="toggle-checkbox"/>
            <label htmlFor="toggle" className="toggle-label">
              <div className="toggle-switch">
                <div className="toggle-knob"></div>
              </div>
              <span className="toggle-text">Dark Mode</span>
            </label>
          </div>
        </div>
      </div>

      <Sidebar isOpen={isSidebarOpen} close={closeSidebar} />
    </div>
  )
}

export default Navbar