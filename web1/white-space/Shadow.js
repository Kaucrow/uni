import { GameObject } from "./GameObject.js";

// Sprite sheet
const shadowSheet = new Image();
shadowSheet.src = './assets/sprites/shadow_lightbulb.png';

export class LightbulbShadow extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: shadowSheet,

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