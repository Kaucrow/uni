import { Router } from 'express';
import { dbPool } from '@global/database.js';
import { queries } from '@const/constants.js';
import { userSchema } from '@schemas/db/security.js';
import type { UUID } from 'crypto';
import { session } from '@components/session.js';

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
        // Create a session with the DB user ID
        session.create(req, res, user.user_id as UUID);

        return res.status(200).json({ message: 'Login successful' });
      }
    }

    // Login failed
    res.status(401).json({ message: 'Invalid credentials' });
  });
});

// Logout endpoint
router.post('/logout', async (req, res) => {
  try {
    await session.destroy(req, res);
    // Logout succeeded
    res.status(200).json({ message: 'Logout successful' });
  } catch (err) {
    // Logout failed
    return res.status(500).json({ message: 'Could not log out' });
  }
});

export default router;