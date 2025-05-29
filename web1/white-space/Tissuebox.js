import { GameObject } from "./GameObject.js";

// Sprite sheet
const tissueboxSheet = new Image();
tissueboxSheet.src = './assets/sprites/tissuebox.png';

export class Tissuebox extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: tissueboxSheet,

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