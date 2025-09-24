import { z } from 'zod';

export const queriesSchema = z.object({
  user: z.object({
    getUserByEmail: z.string(),
    getUserById: z.string()
  }),
});