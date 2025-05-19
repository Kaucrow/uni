import './Home.css'
import CenteredDiv from '../../components/CenteredDiv';
import LogoIcon from '../../components/LogoIcon';

const Home = () => {
  return (
    <CenteredDiv className='home-content' maxWidth='2px'>
      <LogoIcon className='home-logo-icon'/>
      <div className='home-logo-title'>Echoes</div>
      <div className='home-logo-subtitle'>— To what destination do these memories reach? —</div>
      <div className='home-description'>The notebook app that</div>
      <div className='home-description'>listens back.</div>
    </CenteredDiv>
  )
}

export default Home