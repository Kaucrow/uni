import './Register.css';
import Checkbox from '../../../components/Checkbox';
import Input from '../../../components/Input';
import type { CustomError } from '../../../utils/types';
import { Link } from 'react-router-dom';
import { useState } from 'react';

const Register = () => {
  let reqErr: CustomError[] = [];
  let err = {email: '', notFound: ''};
  let [email, setEmail] = useState('');
  let [password, setPassword] = useState('');
  let [rememberMe, setRememberMe] = useState(false);

  function submitForm() { console.log('Form submitted') }

  return (
    <section className="register-container">
      <div className="register-form-box">
        <form onSubmit={submitForm} className="register-form">
          {/* Register section */}
          <div className="register-title-container">
            <p className="register-title">Register</p>
          </div> 

          <Input label="Email address" id="email-input" input={email} setInput={setEmail}/>
          
          <Input label="Name" id="email-input" input={email} setInput={setEmail}/>

          <Input label="Password" id="password-input" input={password} setInput={setPassword}/>

          <Input label="Repeat password" id="password-input" input={password} setInput={setPassword}/>

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
    </section>
  );
};

export default Register;