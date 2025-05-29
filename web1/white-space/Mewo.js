import { GameObject } from "./GameObject.js";

// Sprite sheet
const mewoSheet = new Image();
mewoSheet.src = './assets/sprites/mewo_asleep.png';

export class Mewo extends GameObject {
  constructor(x, y) {
    super({
      x: x,
      y: y,
      width: 32,
      height: 32,
      spriteSheet: mewoSheet,

      actions: {
        sleep: { animates: true },
      },
      defaultAction: 'sleep',

      animations: {
        sleep: {
          frames: 3,
          defaultFrame: 1,
          y: 0,
          direction: GameObject.ANIMATION_DIRS.PINGPONG
        },
      },
      defaultAnimation: 'sleep',
      frameDuration: 1,
      animationDirection: 'forward',
    })
  }

  update(deltaTime) {
    super.update(deltaTime);
  }
}