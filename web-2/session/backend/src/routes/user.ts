import { Router } from 'express';
import type { UserData } from '../types/user.d.js';

const router = Router();

router.get('/user-data', (req, res) => {
  if (req.session.userId) {
    let userData: UserData = {
      name: "Elatla"
    };

    return res.status(200).json(userData);
  }

  res.status(400).json({ message: 'No active session' });
});

export default router;