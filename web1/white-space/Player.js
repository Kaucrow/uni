import { GameObject } from "./GameObject.js";
import { Animator } from "./Animator.js";
import { Vector2 } from "./Vector.js";

export class Player extends GameObject {
  constructor(x, y, z, collisionSystem) {
    super({
      x: x,
      y: y,
      z: z,
      width: 32,
      height: 32,
      spriteSheet: './assets/sprites/omori_walk.png',
      speed: 232,

      actions: {
        idle: { animates: false },
        walk: { animates: true }
      },
      defaultAction: 'idle',

      animator: {
        animations: {
          down: {
            frames: 3,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          left: {
            frames: 3,
            defaultFrame: 1,
            y: 1,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          right: {
            frames: 3,
            defaultFrame: 1,
            y: 2,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          up: {
            frames: 3,
            defaultFrame: 1,
            y: 3,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          }
        },
        defaultAnimation: 'down',
        frameDuration: 0.30,
        animationDirection: 'forward',
      },

      input: {
        animationMappings: {
          movement: {
            x: {
              1: 'right',
              [-1]: 'left'
            },
            y: {
              1: 'down',
              [-1]: 'up'
            }
          }
        },
      },

      collisionSystem,
      colliders: [
        {
          group: 'player',
          interactions: [
            {
              group: 'any',
              onCollide: (source) => {
                source.x = source.oldX;
                source.y = source.oldY;
                source.animator.frameX = 1;
                source.animator.animationTimer = 0;
              }
            },
          ],
          edges: [
            new Vector2([4, 2], [28, 2]),
            new Vector2([28, 2], [28, 32]),
            new Vector2([28, 32], [4, 32]),
            new Vector2([4, 32], [4, 2]),
          ],
        }
      ],

    })
  }
}