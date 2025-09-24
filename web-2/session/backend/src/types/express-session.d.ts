import 'express-session';
import type { UUID } from 'crypto';

declare module 'express-session' {
  interface SessionData {
    sessionId: UUID;
  };
};