import session from 'express-session';
import { session as sessionFileConfig } from '../constants/constants.js';

export const sessionConfig = {
  secret: sessionFileConfig.secret,
  resave: false,
  saveUninitialized: false,
  cookie: {
    maxAge: 3600000,
    secure: false,
    httpOnly: true,
  }
};

export default session(sessionConfig);