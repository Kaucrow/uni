import { GameObject } from "./GameObject.js";
import { SomethingAI } from "./SomethingAI.js";
import { Vector2 } from "./Vector.js";

export class Something extends GameObject {
  constructor(x, y, room) {
    super({
      id: 'something',
      x,
      y,
      z: 5,
      width: 32,
      height: 64,
      spriteSheet: './assets/sprites/something_detailed.png',
      speed: 0,
      room,

      actions: {
        idle: { animates: false }
      },
      defaultAction: 'idle',

      collisionSystem: room.collisionSystem,
      triggers: [
        {
          draw: {},
          group: 'something',
          edges: [
            new Vector2([4, 0], [28, 0]),
            new Vector2([28, 0], [28, 64]),
            new Vector2([28, 64], [4, 64]),
            new Vector2([4, 64], [4, 0]),
          ] 
        }
      ]
    });

    this.AI = new SomethingAI(this);
  }

  update(deltaTime) {
    this.AI.update(deltaTime);
    super.update(deltaTime);
  }

  follow(object) {
    this.AI.follow(object);
  }
}