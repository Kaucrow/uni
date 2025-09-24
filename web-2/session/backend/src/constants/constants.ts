import fs from 'fs';
import toml from 'toml';
import yaml from 'yaml';

import type {
  Config,
  Server,
  Frontend,
  Session,
  Database
} from '@server-types/config.js';

import {
  queriesSchema
} from '@schemas/queries.js';

const config: Config = toml.parse(fs.readFileSync('./src/config/config.toml', 'utf-8'));

export const server: Server = config.server;
export const frontend: Frontend = {
  host: config.frontend.host,
  port: config.frontend.port,
  url: `http://${config.frontend.host}:${config.frontend.port}`
};
export const session: Session = config.session;
export const database: Database = {
  host: config.database.host,
  port: config.database.port,
  name: config.database.name,
  user: config.database.user,
  password: config.database.password,
  url: ''
};

export const queries = queriesSchema.parse(
  yaml.parse(fs.readFileSync('./src/config/queries.yaml', 'utf-8'))
);