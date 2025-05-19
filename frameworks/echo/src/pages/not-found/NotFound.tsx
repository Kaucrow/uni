import './NotFound.css';
import CenteredDiv from '../../components/CenteredDiv';
import invnotfound from '../../assets/invnotfound.png'

const NotFound = () => {
  return (
    <CenteredDiv className='not-found-container'>
      <div className='not-found'>
        <img src={invnotfound} className='not-found-img' />
        <p className='not-found-text'>404 Page Not Found!</p>
      </div>
    </CenteredDiv>
  )
}

export default NotFound