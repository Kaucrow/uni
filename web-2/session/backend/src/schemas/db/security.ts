import { z } from 'zod';

export const userSchema = z.object({
  user_id: z.uuid(),
  email: z.string(),
  passwd: z.string()
});