import { GameObject } from "./GameObject.js";
import { Animator } from "./Animator.js";
import { canvas } from "./canvas.js";

// Sprite sheet
//const walkSheet = new Image();
//walkSheet.src = './assets/sprites/omori_walk.png';

export class Player extends GameObject {
  constructor(x, y, z) {
    super({
      x: x,
      y: y,
      z: z,
      width: 32,
      height: 32,
      spriteSheet: './assets/sprites/omori_walk.png',
      speed: 32,

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
      }
    })
  }

  update(deltaTime) {
    this.updateFromInput(deltaTime);
    super.update(deltaTime);
  }
}