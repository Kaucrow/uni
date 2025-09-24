import type { Express, Request, Response } from 'express';
import expressSession from 'express-session';
import {
  generateKeys as generatePasetoKeys,
  sign as pasetoSign,
  verify as pasetoVerify,
} from 'paseto-ts/v4';
import cookieParser from 'cookie-parser';

import type { UUID } from 'crypto';
import { session as sessionFileConfig } from '@const/constants.js';
import type { Session } from '@server-types/session.js';

class SessionComponent {
  static #instance: SessionComponent;

  private type: 'express' | 'paseto' | undefined = undefined;

  private pasetoKeys: {
    secret: string,
    public: string
  } | undefined = undefined;

  private tokenSessions: Set<UUID> | undefined = undefined;

  private constructor() {}

  public static get instance(): SessionComponent {
    if (!SessionComponent.#instance) {
      SessionComponent.#instance = new SessionComponent();
    }

    return SessionComponent.#instance;
  }

  private async getPasetoPayload(req: Request): Promise<Session> {
    // Get the token from cookies
    const token = req.cookies?.session;

    if (!token) {
      throw new Error("No session token found in cookies"); // Token is missing
    }

    try {
      const { payload }: { payload: Session } = await pasetoVerify(this.pasetoKeys!.public, token);

      if (!this.tokenSessions!.has(payload.userId)) {
        throw new Error("Session expired"); // Session set does not have the user ID (might have expired)
      }

      return payload as Session;  // Verification succeeded
    } catch (err) {
      throw err;
    }
  }

  public enable(app: Express, type: 'express' | 'paseto') {
    this.type = type;

    switch (type) {
      case 'express': {
        // Set up session middleware on the app
        const sessionConfig = {
          secret: sessionFileConfig.secret,
          resave: false,
          saveUninitialized: false,
          cookie: {
            maxAge: 60 * 60 * 100,  // 1 hour
            secure: false,
            httpOnly: true,
          }
        };

        app.use(expressSession(sessionConfig));
        break;
      }
      case 'paseto': {
        // Generate PASETO keys
        const { secretKey, publicKey } = generatePasetoKeys('public');
        this.pasetoKeys = {
          secret: secretKey,
          public: publicKey
        };

        // Initialize token sessions set
        this.tokenSessions = new Set();

        // Set up cookie parser middleware on the app
        app.use(cookieParser());

        break;
      }
    }
  }

  public create(req: Request, res: Response, userId: UUID) {
    if (!this.type) throw new Error("Session has not been initialized. Call session.enable() first");

    switch (this.type) {
      case 'express': {
        req.session.userId = userId;
        break;
      }
      case 'paseto': {
        // Add the session to the sessions set
        this.tokenSessions!.add(userId);

        try {
          // Build the token
          const payload: Session = { userId };
          const token = pasetoSign(this.pasetoKeys!.secret, payload);

          // Set the token as a cookie
          res.cookie('session', token, {
            httpOnly: true,
            secure: false,
            maxAge: 60 * 60 * 1000, // 1 hour
            sameSite: 'strict',
            path: '/',
          });
        } catch (err) {
          throw err;
        }
      }
    }
  }

  public async exists(req: Request): Promise<boolean> {
    if (!this.type) throw new Error("Session has not been initialized. Call session.enable() first");

    switch (this.type) {
      case 'express': {
        // If userId is set in the session, the user is authenticated
        return req.session.userId ? true : false;
      }
      case 'paseto': {
        try {
          await this.getPasetoPayload(req);
          return true;
        } catch (err) {
          console.error(err);
          return false;
        }
      }
    }
  }

  public async get(req: Request): Promise<Session | null> {
    if (!this.type) throw new Error("Session has not been initialized. Call session.enable() first");
 
    switch (this.type) {
      case 'express': {
        if (!req.session.userId) return null;

        return {
          userId: req.session.userId
        }
      }
      case 'paseto': {
        try {
          const payload: Session = await this.getPasetoPayload(req);
          return payload;
        } catch (err) {
          console.error(err);
          return null;
        }
      }
    }
  }

  public async destroy(req: Request, res: Response) {
    if (!this.type) throw new Error("Session has not been initialized. Call session.enable() first");

    switch (this.type) {
      case 'express': {
        req.session.destroy((err) => {
          if (err) throw err
        });
        break;
      }
      case 'paseto': {
        try {
          const payload: Session = await this.getPasetoPayload(req);
          this.tokenSessions!.delete(payload.userId);
          res.clearCookie('session');
        } catch (err) {
          console.error(err);
        }
      }
    }
  }
};

export const session = SessionComponent.instance;