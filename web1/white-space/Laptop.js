import { GameObject } from "./GameObject.js";

// Sprite sheet
const laptopSheet = new Image();
laptopSheet.src = './assets/sprites/laptop_static.png';

export class Laptop extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: laptopSheet,

      actions: {
        static: { animates: true },
      },
      defaultAction: 'static',

      animations: {
        static: {
          frames: 3,
          defaultFrame: 1,
          y: 0,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
      },
      defaultAnimation: 'static',
      frameDuration: 0.8,
      animationCount: 0,
      animationDirection: 'forward',
    })
  }

  update(deltaTime) {
    super.update(deltaTime);
  }
}