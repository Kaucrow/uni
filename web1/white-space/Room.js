import { CollisionSystem } from "./CollisionSystem.js"

export class Room {
  constructor(config) {
    this.collisionSystem = new CollisionSystem();
    this.width = config.width;
    this.height = config.height;
    this.camera = config.camera || undefined;
  }
}