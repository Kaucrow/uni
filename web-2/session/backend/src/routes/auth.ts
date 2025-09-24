import { Router } from 'express';
import { dbPool } from '@global/database.js';
import { queries } from '@const/constants.js';
import { userSchema } from '@schemas/db/security.js';
import { sessions } from '@global/session.js';
import crypto, { type UUID } from 'crypto';

const router = Router();

// Login endpoint
router.post('/login', (req, res) => {
  const { email, password } = req.body;

  dbPool.query(queries.user.getUserByEmail, [email], (err, results) => {
    if (err) throw err;

    // If a user was found, validate their password
    if (results.rowCount) {
      const user = userSchema.parse(results.rows[0]);

      // If the password matches, create the session
      if (user.passwd === password) {
        // Create a session ID and store it as the key to our database user ID
        const sessionId = crypto.randomUUID();
        sessions.set(sessionId, user.user_id as UUID);

        // Set the session cookie
        req.session.sessionId = sessionId;
        return res.status(200).json({ message: 'Login successful' });
      }
    }

    // Login failed
    res.status(401).json({ message: 'Invalid credentials' });
  });
});

// Logout endpoint
router.post('/logout', (req, res) => {
  req.session.destroy((err) => {
    if (err) {
      return res.status(500).json({ message: 'Could not log out' });
    }
    res.status(200).json({ message: 'Logout successful' });
  });
});

export default router;