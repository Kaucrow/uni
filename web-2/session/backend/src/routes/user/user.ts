import { Router } from 'express';
import { dbPool } from '@global/database.js';
import { queries } from '@const/constants.js';
import { userSchema } from '@schemas/db/security.js';
import type { UserData } from './responses.js';
import { session } from '@components/session.js';

const router = Router();

router.get('/user-data', async (req, res) => {
  const sessionData = await session.get(req);

  // Error if the user isn't authenticated
  if (!sessionData || !sessionData.userId) {
    return res.status(401).json({ message: 'User is not authenticated' });
  }

  // If they are, get the user from DB
  const userId = sessionData.userId;
  dbPool.query(queries.user.getUserById, [userId], (err, results) => {
    if (err) throw err;

    const user = userSchema.parse(results.rows[0]); 

    const userData: UserData = {
      name: user.name,
      surname: user.surname  
    };

    // Return the user data
    return res.status(200).json(userData);
  });
});

export default router;