import './Sidebar.css';
import { Link } from 'react-router-dom';
import LogoIcon from './LogoIcon';
import { useAuth } from '../hooks/useAuth';

interface SidebarProps {
  isOpen: boolean;
  close: () => void;
}

const Sidebar: React.FC<SidebarProps> = ({ isOpen, close }) => {
  const { userLoggedIn } = useAuth();

  return (
    <div className={`sidebar ${isOpen ? 'open' : ''}`}>
      <div className="sidebar-content">
        <div className="sidebar-header">
          <Link to='/' className="sidebar-logo-link">
            {/* Logo Icon */}
            <LogoIcon />
            {/* Logo Text */}
            <h1 className="sidebar-logo-text">Echoes</h1>
          </Link>

          {/* Close button */}
          <button onClick={close} className="close-button">
            <svg className="close-icon" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
              <path className="stroke-current" strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M6 18L18 6M6 6l12 12"></path>
            </svg>
            <div className="close-effect"></div>
          </button>
        </div>

        {/* Navbar */}
        <nav className="nav">
          <Link to='/' onClick={close} className="nav-link">
            <svg className="nav-icon" xmlns="http://www.w3.org/2000/svg" width="23" height="23" viewBox="0 0 576 512">
              <path d="M575.8 255.5c0 18-15 32.1-32 32.1l-32 0 .7 160.2c0 2.7-.2 5.4-.5 8.1l0 16.2c0 22.1-17.9 40-40 40l-16 0c-1.1 0-2.2 0-3.3-.1c-1.4 .1-2.8 .1-4.2 .1L416 512l-24 0c-22.1 0-40-17.9-40-40l0-24 0-64c0-17.7-14.3-32-32-32l-64 0c-17.7 0-32 14.3-32 32l0 64 0 24c0 22.1-17.9 40-40 40l-24 0-31.9 0c-1.5 0-3-.1-4.5-.2c-1.2 .1-2.4 .2-3.6 .2l-16 0c-22.1 0-40-17.9-40-40l0-112c0-.9 0-1.9 .1-2.8l0-69.7-32 0c-18 0-32-14-32-32.1c0-9 3-17 10-24L266.4 8c7-7 15-8 22-8s15 2 21 7L564.8 231.5c8 7 12 15 11 24z"/>
            </svg>
            <span>Home</span>
          </Link>
          {!userLoggedIn && <div className="nav-conditional">
            <Link to='/auth/login' onClick={close} className="nav-link">
              <svg className="nav-icon" xmlns="http://www.w3.org/2000/svg" width="23" height="23" viewBox="0 0 512 512">
                <path d="M217.9 105.9L340.7 228.7c7.2 7.2 11.3 17.1 11.3 27.3s-4.1 20.1-11.3 27.3L217.9 406.1c-6.4 6.4-15 9.9-24 9.9c-18.7 0-33.9-15.2-33.9-33.9l0-62.1L32 320c-17.7 0-32-14.3-32-32l0-64c0-17.7 14.3-32 32-32l128 0 0-62.1c0-18.7 15.2-33.9 33.9-33.9c9 0 17.6 3.6 24 9.9zM352 416l64 0c17.7 0 32-14.3 32-32l0-256c0-17.7-14.3-32-32-32l-64 0c-17.7 0-32-14.3-32-32s14.3-32 32-32l64 0c53 0 96 43 96 96l0 256c0 53-43 96-96 96l-64 0c-17.7 0-32-14.3-32-32s14.3-32 32-32z"/>
              </svg>
              <span>Login</span>
            </Link>
            <Link to='/auth/register' onClick={close} className="nav-link">
              <svg className="nav-icon" xmlns="http://www.w3.org/2000/svg" width="23" height="23" viewBox="0 0 512 512">
                <path d="M362.7 19.3L314.3 67.7 444.3 197.7l48.4-48.4c25-25 25-65.5 0-90.5L453.3 19.3c-25-25-65.5-25-90.5 0zm-71 71L58.6 323.5c-10.4 10.4-18 23.3-22.2 37.4L1 481.2C-1.5 489.7 .8 498.8 7 505s15.3 8.5 23.7 6.1l120.3-35.4c14.1-4.2 27-11.8 37.4-22.2L421.7 220.3 291.7 90.3z"/>
              </svg>
              <span>Register</span>
            </Link>
          </div>}
          <Link to="#!" onClick={close} className="nav-link">
            <svg className="nav-icon" xmlns="http://www.w3.org/2000/svg" width="23" height="23" viewBox="0 0 512 512">
              <path d="M256 8C119 8 8 119.1 8 256c0 137 111 248 248 248s248-111 248-248C504 119.1 393 8 256 8zm0 110c23.2 0 42 18.8 42 42s-18.8 42-42 42-42-18.8-42-42 18.8-42 42-42zm56 254c0 6.6-5.4 12-12 12h-88c-6.6 0-12-5.4-12-12v-24c0-6.6 5.4-12 12-12h12v-64h-12c-6.6 0-12-5.4-12-12v-24c0-6.6 5.4-12 12-12h64c6.6 0 12 5.4 12 12v100h12c6.6 0 12 5.4 12 12v24z"/>
            </svg>
            <span>About</span>
          </Link>
        </nav>
      </div>
    </div>
  );
};

export default Sidebar;