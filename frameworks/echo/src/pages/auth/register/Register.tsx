import './Register.css';
import Input from '../../../components/Input';
import CenteredDiv from '../../../components/CenteredDiv';
import type { CustomError } from '../../../utils/types';
import { Link, Navigate } from 'react-router-dom';
import { useState } from 'react';
import { useAuth } from '../../../hooks/useAuth';
import { doCreateUserWithEmailAndPassword } from '../../../firebase/auth';
import type { FormEvent } from 'react';

const Register = () => {
  const { userLoggedIn } = useAuth();

  if (userLoggedIn) {
    return <Navigate to={'/'} replace />
  }

  let reqErr: CustomError[] = [];
  let err = {email: '', notFound: ''};
  let [email, setEmail] = useState('');
  let [name, setName] = useState('');
  let [password, setPassword] = useState('');
  let [repeatPassword, setRepeatPassword] = useState('');
  let [isRegistering, setIsRegistering] = useState(false);

  const onSubmit = async (e: FormEvent<HTMLFormElement>) => {
    console.log('registering');
    e.preventDefault();
    if (!isRegistering) {
      try {
        setIsRegistering(true);
        await doCreateUserWithEmailAndPassword(email, password);
      } catch (err) {
        console.log(err);
        setIsRegistering(false);
      }
    }
  }

  return (
    <CenteredDiv className="register-container">
      <div className="register-form-box">
        <form onSubmit={onSubmit} className="register-form">
          {/* Register section */}
          <div className="register-title-container">
            <p className="register-title">Register</p>
          </div> 

          <Input label="Email address" id="email-input" input={email} setInput={setEmail}/>
          
          <Input label="Name" id="name-input" input={name} setInput={setName}/>

          <Input label="Password" id="password-input" input={password} setInput={setPassword}/>

          <Input label="Repeat password" id="repeat-password-input" input={repeatPassword} setInput={setRepeatPassword}/>

          {err.email && <p className="error-message">{err.email}</p>}
          {err.notFound && <p className="error-message">{err.notFound}</p>}

          {/* register button */}
          <div className="text-center">
            <button type="submit" className="register-btn">Register</button>
            <p className="register-text">
              Already have an account? <Link to="/auth/login" className="register-link">Sign In</Link>
            </p>
          </div>
        </form>
      </div>
    </CenteredDiv>
  );
};

export default Register;