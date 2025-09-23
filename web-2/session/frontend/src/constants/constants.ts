import config from "$config/config.toml";
import type { BackendConfig } from "$types";

export const backend: BackendConfig = {
  host: config.backend.host,
  port: config.backend.port,
  url: `http://${config.backend.host}:${config.backend.port}`
};