import './Login.css';
import Checkbox from '../../../components/Checkbox';
import Input from '../../../components/Input';
import CenteredDiv from '../../../components/CenteredDiv';
import { Link, Navigate } from 'react-router-dom';
import { useState } from 'react'
import { useAuth } from '../../../hooks/useAuth';
import { doSignInWithEmailAndPassword, doSignInWithGoogle } from '../../../firebase/auth';
import type { FormEvent } from 'react';

const Login = () => {
  const { userLoggedIn } = useAuth();

  if (userLoggedIn) {
    return <Navigate to={'/'} replace />
  }

  const [isSigningIn, setIsSigningIn] = useState(false);
  let [email, setEmail] = useState('');
  let [password, setPassword] = useState('');

  const onSubmit = async (e: FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    if (!isSigningIn) {
      try {
        setIsSigningIn(true);
        await doSignInWithEmailAndPassword(email, password);
      } catch (err) {
        console.log(err);
        setIsSigningIn(false);
      }
    }
  }

  const onGoogleSignIn = (e: FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    if(!isSigningIn) {
      setIsSigningIn(true);
      doSignInWithGoogle().catch(_ => {
        setIsSigningIn(false)
      });
    }
  }

  let err = {email: '', notFound: ''};
  let [rememberMe, setRememberMe] = useState(false);

  return (
    <CenteredDiv className="login-container">
      <div className="login-form-box">
        <form onSubmit={onSubmit} className="login-form">
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

          <Input
            required
            label="Email address"
            id="email-input"
            input={email}
            setInput={setEmail}
          />

          <Input
            required
            label="Password"
            id="password-input"
            input={password}
            setInput={setPassword}
          />

          {err.email && <p className="error-message">{err.email}</p>}
          {err.notFound && <p className="error-message">{err.notFound}</p>}

          <div className="remember-me">
            {/* Remember me checkbox */}
            <Checkbox
              label="Remember Me"
              checked={rememberMe}
              setChecked={setRememberMe}
            />

            <Link to="#!" className="forgot-password">Forgot password?</Link>
          </div>

          {/* Login button */}
          <div className="text-center">
            <button type="submit" className="login-btn">Login</button>
            <p className="register-text">
              New user? <Link to="/auth/register" className="register-link">Register</Link>
            </p>
          </div>
        </form>
      </div>
    </CenteredDiv>
  );
};

export default Login;