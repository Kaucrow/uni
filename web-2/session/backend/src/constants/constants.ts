import fs from 'fs';
import toml from 'toml';

import type { Config, Server, Frontend, Session } from '../types/config.js';

const config: Config = toml.parse(fs.readFileSync('./src/config/config.toml', 'utf-8'));

export const server: Server = config.server;
export const frontend: Frontend = {
  host: config.frontend.host,
  port: config.frontend.port,
  url: `http://${config.frontend.host}:${config.frontend.port}`
};
export const session: Session = config.session;