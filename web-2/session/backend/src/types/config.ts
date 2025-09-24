export interface Config {
  server: Server,
  frontend: {
    host: string,
    port: number
  },
  session: Session,
  database: {
    host: string,
    port: number,
    name: string,
    user: string,
    password: string
  },
};

export interface Server {
  port: number
};

export interface Frontend {
  host: string,
  port: number,
  url: string
};

export interface Session {
  secret: string
};

export interface Database {
  host: string,
  port: number,
  name: string,
  user: string,
  password: string,
  url: string
};