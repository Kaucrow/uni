import { GameObject } from "./GameObject.js";
import { LightSource } from "./LightSource.js";
import { Vector2 } from "./Vector.js";

export class Key extends GameObject {
  constructor(x, y, room) {
    super({
      id: 'key',
      x,
      y,
      z: 2,
      width: 32,
      height: 32,
      spriteSheet: './assets/sprites/key.png',
      room,

      triggers: [
        {
          group: 'key',
          interactsWith: 'player',
          edges: [
            new Vector2([4, 22], [30, 22]),
            new Vector2([30, 22], [30, 32]),
            new Vector2([30, 32], [4, 32]),
            new Vector2([4, 32], [4, 22]),
          ],
        }
      ],

      collisionSystem: room.collisionSystem,
    });

    this.light = new LightSource(x + this.width / 2, y + this.height / 2, 25, "#FFFFFF", {
      low: 0.7,    // 70% of base radius at lowest
      high: 1.3,   // 130% of base radius at peak
      speed: 0.5,
    });
  }

  Destroy() {
    // Remove from keys array
    if (this.room.keys) {
      this.room.keys = this.room.keys.filter(key => key.instance !== this);
    }
    
    // Remove from collision system if needed
    if (this.collisionSystem) {
      this.collisionSystem.removeObject(this);
    }
    
    // Clean up light source if needed
    if (this.light && this.room.lightSystem) {
      this.room.lightSystem.removeLight(this.light);
    }
  }
}