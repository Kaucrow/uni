import { GameObject } from "./GameObject.js";
import { CollisionSystem } from "./CollisionSystem.js";
import { RedHandAI } from "./RedHandAI.js";
import { Animator } from "./Animator.js";
import { Vector2 } from "./Vector.js";

export class RedHand extends GameObject {
  constructor(x, y, collisionSystem) {
    super({
      id: 'redhand',
      x,
      y,
      z: 5,
      width: 32,
      height: 32,
      spriteSheet: './assets/sprites/redhand.png',
      speed: 32,

      actions: {
        idle: { animates: true }
      },
      defaultAction: 'idle',

      animator: {
        animations: {
          down: {
            frames: 3,
            frameDuration: 0.30,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          left: {
            frames: 3,
            frameDuration: 0.30,
            y: 1,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          right: {
            frames: 3,
            frameDuration: 0.30,
            y: 2,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          up: {
            frames: 3,
            frameDuration: 0.30,
            y: 3,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
        }
      },
      movementAnimator: {},

      collisionSystem,
      colliders: [
        {
          group: 'solid',
          interactions: [
            {
              group: 'solid',
              onCollide: (source) => CollisionSystem.FUNCS.STOP(source) 
            },
          ],
          edges: [
            new Vector2([8, 2], [26, 2]),
            new Vector2([26, 2], [26, 30]),
            new Vector2([26, 30], [8, 30]),
            new Vector2([8, 30], [8, 2]),
          ]
        }
      ]
    });

    this.AI = new RedHandAI(this);
  }

  update(deltaTime) {
    this.AI.update(deltaTime);
    super.update(deltaTime);
  }
}