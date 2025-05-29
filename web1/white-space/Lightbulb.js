import { GameObject } from "./GameObject.js";

// Sprite sheet
const lightbulbSheet = new Image();
lightbulbSheet.src = './assets/sprites/lightbulb_dim.png';

export class Lightbulb extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: lightbulbSheet,

      actions: {
        glow: { animates: true },
      },
      defaultAction: 'glow',

      animations: {
        glow: {
          frames: 3,
          defaultFrame: 1,
          y: 0,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
      },
      defaultAnimation: 'glow',
      frameDuration: 0.6,
      animationDirection: 'forward',
    })
  }

  update(deltaTime) {
    super.update(deltaTime);
  }
}