import express from 'express';
import sessionMiddleware from './config/session.ts';
import authRoutes from './routes/auth.ts';

const app = express();

// Session middleware
app.use(sessionMiddleware);

// Middleware to parse JSON from request body
app.use(express.json());

// Auth routes
app.use('/auth', authRoutes);

// Health check endpoint
app.get('/health', (req, res) => {
  res.status(200).json({ status: 'OK', timestamp: new Date().toISOString() });
});

// 404 handler
app.use(function(req, res, next) {
  res.status(404);

  // Respond with json
  if (req.accepts('json')) {
    res.json({ error: 'Not found' });
    return;
  }

  // Default to plain-text. send()
  res.type('txt').send('Not found');
});

export default app;