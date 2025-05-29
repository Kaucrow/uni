import { GameObject } from "./GameObject.js";
import { Controller } from "./Controller.js";
import { canvas } from "./canvas.js";

// Sprite sheet
const walkSheet = new Image();
walkSheet.src = './assets/sprites/omori_walk.png';

export class Player extends GameObject {
  constructor(x, y) {
    const controller = new Controller({
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
      }
    });

    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: walkSheet,

      actions: {
        idle: { animates: false },
        walk: { animates: true }
      },
      defaultAction: 'idle',

      animations: {
        down: {
          frames: 3,
          defaultFrame: 1,
          y: 0,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
        left: {
          frames: 3,
          defaultFrame: 1,
          y: 1,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
        right: {
          frames: 3,
          defaultFrame: 1,
          y: 2,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
        up: {
          frames: 3,
          defaultFrame: 1,
          y: 3,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        }
      },
      defaultAnimation: 'down',
      frameDuration: 0.30,
      animationDirection: 'forward',

      speed: 32,

      controller: controller
    })

    this.controller = controller;
  }

  update(deltaTime) {
    this.updateFromController(deltaTime);
    super.update(deltaTime);
  }
}