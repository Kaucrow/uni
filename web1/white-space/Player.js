import { GameObject } from "./GameObject.js";
import { Animator } from "./Animator.js";
import { Vector2 } from "./Vector.js";

export class Player extends GameObject {
  constructor(x, y, z, collisionSystem) {
    super({
      id: 'player',
      x: x,
      y: y,
      z: z,
      width: 32,
      height: 32,

      actions: {
        idle: {
          animates: false,
          spriteSheet: './assets/sprites/omori_walk.png',
        },
        walk: {
          animates: true,
          spriteSheet: './assets/sprites/omori_walk.png',
        },
        run: {
          animates: true,
          spriteSheet: './assets/sprites/omori_run.png',
        }
      },
      defaultAction: 'idle',

      animator: {
        animations: {
          walkDown: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkLeft: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 1,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkRight: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 2,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkUp: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 3,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          runDown: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runLeft: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 1,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runRight: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 2,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runUp: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 3,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
        },
        animationDirection: 'forward',
      },

      input: {
        animationMappings: {
          movement: {
            x: {
              1: 'Right',
              [-1]: 'Left'
            },
            y: {
              1: 'Down',
              [-1]: 'Up'
            }
          }
        },
        walkSpeed: 96,
        runSpeed: 224,
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
                source.setAction('idle');
                source.animator.setAnimation(source.animator.currentAnimation.replace("run", "walk"));
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