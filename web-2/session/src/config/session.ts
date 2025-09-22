import session from 'express-session';
import { SESSION_SECRET } from './constants.ts';

export const sessionConfig = {
  secret: SESSION_SECRET,
  resave: false,
  saveUninitialized: false,
  cookie: {
    maxAge: 3600000,
    secure: false,
    httpOnly: true,
  }
};

export default session(sessionConfig);