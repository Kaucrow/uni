import { Pool } from 'pg';
import { database } from '@const/constants.js';

export const dbPool = new Pool({
  host: database.host,
  port: database.port,
  database: database.name,
  user: database.user,
  password: database.password
});