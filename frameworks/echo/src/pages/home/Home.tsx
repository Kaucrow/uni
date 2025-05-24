import './Home.css'
import CenteredDiv from '../../components/CenteredDiv';
import LogoIcon from '../../components/LogoIcon';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faGithub } from '@fortawesome/free-brands-svg-icons';
import { useAuth } from '../../hooks/useAuth';

const Home = () => {
  const { userLoggedIn } = useAuth();

  return (
    <div className="centered-div-container">
      <CenteredDiv className='home-content' paddingTop='10vh'>
        <LogoIcon className='home-logo-icon'/>
        <div className='home-logo-title'>Echoes</div>
        <div className='home-logo-subtitle'>— To what destination do these memories reach? —</div>
        <div className='home-description'>The notebook app that</div>
        <div className='home-description'>listens back.</div>
      </CenteredDiv>

      <a href="https://github.com/Kaucrow/uni/tree/dev/frameworks/echo" className="about-badge">
        <FontAwesomeIcon className='about-github-icon' icon={faGithub} />
        <p className='about-text'>Kaucrow - 2025</p>
        { userLoggedIn ? 
          <p>You are logged in</p> :
          <p>You are not logged in</p>
        }
      </a>
    </div>
  )
}

export default Home