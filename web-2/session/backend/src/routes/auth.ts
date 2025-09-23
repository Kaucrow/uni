import { Router } from 'express';

const router = Router();

router.post('/login', (req, res) => {
  const { email, password } = req.body;

  if (email === 'elatla@elatla.com' && password === 'elatla') {
    req.session.userId = 'user_123';
    return res.status(200).json({ message: 'Login successful' });
  }

  res.status(401).json({ message: 'Invalid credentials' });
});

router.post('/logout', (req, res) => {
  req.session.destroy((err) => {
    if (err) {
      return res.status(500).json({ message: 'Could not log out' });
    }
    res.status(200).json({ message: 'Logout successful' });
  });
});

export default router;