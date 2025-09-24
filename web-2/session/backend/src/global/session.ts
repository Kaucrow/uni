import type { UUID } from "crypto";

export const sessions: Map<UUID, UUID> = new Map();