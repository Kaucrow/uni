export interface Config {
  server: Server,
  frontend: {
    host: string,
    port: number
  },
  session: Session
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