import { GameObject } from "./GameObject.js";

// Sprite sheet
const sketchbookSheet = new Image();
sketchbookSheet.src = './assets/sprites/sketchbook.png';

export class Sketchbook extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: sketchbookSheet,

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