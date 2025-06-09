import { Player } from "../Player.js";
import { Rect } from "../Rect.js";
import { GameObject } from "../GameObject.js";
import { GameCamera } from "../GameCamera.js";
import { Animator } from "../Animator.js";
import { Vector2 } from "../Vector.js";
import { ctx, canvas } from "../canvas.js";
import { Room } from "../Room.js";

export class WhiteSpace extends Room {
  constructor() {
    super({
      width: 2000,
      height: 2000
    });

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
      },

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'npc',
          edges: [
            new Vector2([4, 4], [30, 4]),
            new Vector2([30, 4], [30, 16]),
            new Vector2([30, 16], [4, 16]),
            new Vector2([4, 16], [4, 4]),
          ]
        }
      ],
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
      },

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'object',
          edges: [
            new Vector2([5, 4], [28, 4]),
            new Vector2([28, 4], [28, 20]),
            new Vector2([28, 20], [5, 20]),
            new Vector2([5, 20], [5, 4]),
          ]
        }
      ]
    });

    const sketchbook = new GameObject({
      x: rectX + 96,
      y: rectY,
      z: 0,
      spriteSheet: './assets/sprites/sketchbook.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'object',
          edges: [
            new Vector2([5, 4], [28, 4]),
            new Vector2([28, 4], [28, 16]),
            new Vector2([28, 16], [5, 16]),
            new Vector2([5, 16], [5, 4]),
          ]
        }
      ],
    });

    const tissuebox = new GameObject({
      x: rectX + 96,
      y: rectY + 64,
      z: 0,
      spriteSheet: './assets/sprites/tissuebox.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'object',
          edges: [
            new Vector2([5, 6], [28, 6]),
            new Vector2([28, 6], [28, 18]),
            new Vector2([28, 18], [5, 18]),
            new Vector2([5, 18], [5, 6]),
          ]
        }
      ],
    });

    const door = new GameObject({
      x: rectX,
      y: rectY - 96,
      z: 0,
      spriteSheet: './assets/sprites/door.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'object',
          edges: [
            new Vector2([5, 0], [28, 0]),
            new Vector2([28, 0], [28, 22]),
            new Vector2([28, 22], [5, 22]),
            new Vector2([5, 22], [5, 0]),
          ]
        }
      ],
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
      this.player = new Player(rectX + 64, rectY + 32, 5, this.collisionSystem),
    ];

    this.objects.sort((a, b) => a.z - b.z);

    this.bgMusic = {
      url: 'assets/music/bg_white_space.mp3',
      loopEnd: 27.9
    };

    this.camera = new GameCamera(this.player, canvas.width, canvas.height, this.width, this.height);
  }

  update(deltaTime) {
    this.camera.update();

    this.#draw(deltaTime);

    if (this.camera.x <= -this.width / 2) {
      this.player.x += this.width;
    } else if (this.camera.x >= this.width / 2) {
      this.player.x -= this.width;
    } else if (this.camera.y <= -this.height / 2) {
      this.player.y += this.height;
    } else if (this.camera.y >= this.height / 2) {
      this.player.y -= this.height;
    }

    this.camera.resetTransform(ctx);
  }

  #draw(deltaTime) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    this.camera.applyTransform(ctx);

    this.objects.forEach(obj => {
      obj.update(deltaTime);
      obj.draw(ctx); 
    });

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y + 2);
    ctx.lineTo(this.lightbulb.x + 17, -128);
    ctx.strokeStyle = '#000000ff';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, -128);
    ctx.lineTo(this.lightbulb.x + 17, -160);
    ctx.strokeStyle = '#000000cc';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, -160);
    ctx.lineTo(this.lightbulb.x + 17, -192);
    ctx.strokeStyle = '#00000099';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, -192);
    ctx.lineTo(this.lightbulb.x + 17, -224);
    ctx.strokeStyle = '#00000066';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, -224);
    ctx.lineTo(this.lightbulb.x + 17, -256);
    ctx.strokeStyle = '#00000033';
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