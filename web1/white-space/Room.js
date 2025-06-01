import { Player } from "./Player.js";
import { Rect } from "./Rect.js";
import { GameObject } from "./GameObject.js";
import { Animator } from "./Animator.js";
import { ctx, canvas } from "./canvas.js";

export class Room {
  constructor() {
    const rectWidth = 128;
    const rectHeight = 96;
    const rectX = canvas.width / 2 - rectWidth / 2;
    const rectY = canvas.height / 2 - rectHeight / 2;

    const lightbulbX = rectX + 64;
    const lightbulbY = rectY - 72;

    this.canvasWidth = canvas.width;
    this.canvasHeight = canvas.height;

    const lightbulb = new GameObject({
      x: lightbulbX,
      y: lightbulbY,
      z: 10,
      spriteSheet: './assets/sprites/lightbulb_dim.png',

      actions: {
        idle: { animates: true },
      },

      animator: {
        animations: {
          idle: {
            frames: 3,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          }
        },
        frameDuration: 0.6
      },
    });

    const mewo = new GameObject({
      x: rectX,
      y: rectY + 96,
      z: 0,
      spriteSheet: './assets/sprites/mewo_asleep.png',

      actions: {
        idle: { animates: true },
      },

      animator: {
        animations: {
          idle: {
            frames: 3,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          }
        },
        frameDuration: 1
      }
    });

    const laptop = new GameObject({
      x: rectX + 32,
      y: rectY,
      z: 0,
      spriteSheet: './assets/sprites/laptop_static.png',
      
      actions: {
        idle: { animates: true },
      },

      animator: {
        animations: {
          idle: {
            frames: 3,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          }
        },
        frameDuration: 0.8
      }
    });

    const sketchbook = new GameObject({
      x: rectX + 96,
      y: rectY,
      z: 0,
      spriteSheet: './assets/sprites/sketchbook.png',
    });

    const tissuebox = new GameObject({
      x: rectX + 96,
      y: rectY + 64,
      z: 0,
      spriteSheet: './assets/sprites/tissuebox.png',
    });

    const door = new GameObject({
      x: rectX,
      y: rectY - 96,
      z: 0,
      spriteSheet: './assets/sprites/door.png',
    });

    const lightbulbShadow = new GameObject({
      x: lightbulb.x,
      y: lightbulb.y + 32,
      z: 0,
      spriteSheet: './assets/sprites/shadow_lightbulb.png',
    });

    this.objects = [
      this.rect = new Rect(rectX, rectY, -1, 128, 96),
      this.sketchbook = sketchbook,
      this.tissuebox = tissuebox,
      this.door = door,
      this.lightbulbShadow = lightbulbShadow,
      this.lightbulb = lightbulb,
      this.mewo = mewo,
      this.laptop = laptop,
      this.player = new Player(rectX + 64, rectY + 32, 5),
    ];

    this.objects.sort((a, b) => a.z - b.z);

    this.bgMusic = {
      url: 'assets/music/bg_white_space.mp3',
      loopEnd: 27.9
    };
  }

  update(deltaTime) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    this.objects.forEach(obj => {
      obj.update(deltaTime);
      obj.draw(ctx); 
    });

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y + 2);
    ctx.lineTo(this.lightbulb.x + 17, 0);
    ctx.strokeStyle = 'black';
    ctx.lineWidth = 1;
    ctx.stroke();
  }

  adjustCanvas() {
    let widthDiff = canvas.width - this.canvasWidth;
    let heightDiff = canvas.height - this.canvasHeight;

    for (const obj of this.objects) {
      obj.x += widthDiff / 2;
      obj.y += heightDiff / 2;
    }

    this.canvasWidth = canvas.width;
    this.canvasHeight = canvas.height;
  }
}