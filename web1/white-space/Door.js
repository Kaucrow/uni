import { GameObject } from "./GameObject.js";

// Sprite sheet
const doorSheet = new Image();
doorSheet.src = './assets/sprites/door.png';

export class Door extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 33,
      spriteSheet: doorSheet,

      animations: {
        idle: {
          frames: 1,
          defaultFrame: 0,
          y: 0,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
      },
    })
  }
}