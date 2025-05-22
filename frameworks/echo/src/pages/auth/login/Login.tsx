import './Login.css';
import Checkbox from '../../../components/Checkbox';
import Input from '../../../components/Input';
import type { CustomError } from '../../../utils/types';
import { useState } from 'react';

const Login = () => {
  let reqErr: CustomError[] = [];
  let err = {email: '', notFound: ''};
  let [email, setEmail] = useState('');
  let [password, setPassword] = useState('');
  let [rememberMe, setRememberMe] = useState(false);

  function submitForm() { console.log('Form submitted') }

  return (
    <section className="login-container">
      <div className="login-form-box">
        <form onSubmit={submitForm} className="login-form">
          {/* Sign in section */}
          <div className="signin-with">
            <p className="signin-with-text">Sign in with</p>
            {/* Google button */}
            <button type="button" className="google-btn">
              <span>
                <svg width="30px" height="30px" viewBox="0 0 45 45" xmlns="http://www.w3.org/2000/svg" className="inline-block align-middle">
                  {/* SVG paths */}
                </svg>
              </span>
            </button>
          </div>

          {/* Separator */}
          <div className="separator">
            <p className="separator-text">Or</p>
          </div>

          <Input label="Email address" id="email-input" input={email} setInput={setEmail}/>
          
          <Input label="Password" id="password-input" input={password} setInput={setPassword}/>

          {err.email && <p className="error-message">{err.email}</p>}
          {err.notFound && <p className="error-message">{err.notFound}</p>}

          <div className="remember-me">
            {/* Remember me checkbox */}
            <Checkbox
              label="Remember Me"
              checked={rememberMe}
              setChecked={setRememberMe}
            />

            <a href="#!" className="forgot-password">Forgot password?</a>
          </div>
          {/* Login button */}
          <div className="text-center">
            <button type="submit" className="login-btn">Login</button>
            <p className="register-text">
              New user? <a href="/auth/register" className="register-link">Register</a>
            </p>
          </div>
        </form>
      </div>
    </section>
  );
};

export default Login;