import { Player } from "../Player.js";
import { RedHand } from "../RedHand.js";
import { Something } from "../Something.js";
import { Rect } from "../Rect.js";
import { GameObject } from "../GameObject.js";
import { GameCamera } from "../GameCamera.js";
import { Animator } from "../Animator.js";
import { Vector2 } from "../Vector.js";
import { ctx, canvas } from "../canvas.js";
import { Room } from "../Room.js";
import { Dialogue } from "../Dialogue.js";
import { Trigger } from "../Trigger.js";
import { Vignette } from "../Vignette.js";
import { Flash } from "../Flash.js";
import { Lives } from "../Lives.js";
import { TextFade } from "../FadeText.js";
import { audioPlayer } from "../Game.js";

export class WhiteSpace extends Room {
  constructor() {
    super({
      width: 3000,
      height: 3000,
      dialogues: {
        'mewo': [
          new Dialogue('Miau? (Esperando a que algo suceda?)', Dialogue.SPEED.NORMAL),
        ],
        'laptop': [
          new Dialogue("Es tu laptop. No hay mas que estatica en la pantalla.", Dialogue.SPEED.NORMAL),
        ],
        'sketchbook': [
          new Dialogue("Le echas un vistazo a las paginas de tu cuaderno de dibujo...", Dialogue.SPEED.NORMAL),
          new Dialogue("Entre tus bocetos, descubres un dibujo que no recuerdas haber hecho.", Dialogue.SPEED.NORMAL),
          new Dialogue(". . .", Dialogue.SPEED.ULTRASLOW, () => {
            audioPlayer.stopAll();
            audioPlayer.playOrQueue('anxiety', { loop: true });
            this.vignette.transitionTo({ intensity: 1, radius: 0.8, duration: 1 });
          }),
          new Dialogue("Sientes una inquietante presencia acercarse.", Dialogue.SPEED.SLOW, (room) => {
            if (!room.something) {
              room.spawnSomething = true;
            }
          }),
        ],
        'tissuebox': [
          new Dialogue("Una caja con papel desechable para secar tus penas.", Dialogue.SPEED.NORMAL),
        ],
        'door': [
          new Dialogue("Esta puerta blanca proyecta una tenue sombra.", Dialogue.SPEED.NORMAL),
          new Dialogue("No parece abrirse.", Dialogue.SPEED.NORMAL),
        ]
      }
    });

    audioPlayer.load('white-space', './assets/sfx/bgm/white_space.mp3');
    audioPlayer.load('anxiety', './assets/sfx/bgm/anxiety.mp3');
    audioPlayer.load('health-fade', './assets/sfx/se/health_fade.mp3');
    audioPlayer.load('damage', './assets/sfx/se/damage.mp3');
    audioPlayer.playOrQueue('white-space', { loop: true, loopEnd: 27.9 });

    const rectWidth = 128;
    const rectHeight = 96;
    const rectX = this.width / 2;
    const rectY = this.height / 2;

    const lightbulbX = rectX + 64;
    const lightbulbY = rectY - 72;

    this.canvasWidth = canvas.width;
    this.canvasHeight = canvas.height;

    const redHandCount = Math.round(Math.random() * 4) + 8;
    this.redHands = [];
    for (let i = 0; i < redHandCount; i++) {
      // Allow x-axis alignment
      if (Math.random() < 0.5) {
        const randomX = Math.random() < 0.5 
          ? Math.round(Math.random() * (this.width / 2 - 1000) + 500) 
          : Math.round(Math.random() * (this.width / 2 - 1000) + this.width / 2 + 500);

        const randomY = Math.round(Math.random() * (this.height - 1000) + 500);

        const redHand = new RedHand(randomX, randomY, this.collisionSystem);
        this.redHands.push(redHand);
      // Allow y-axis alignment
      } else {
        const randomX = Math.round(Math.random() * (this.width - 1000) + 500);

        const randomY = Math.random() < 0.5 
          ? Math.round(Math.random() * (this.height/2 - 1000) + 500) 
          : Math.round(Math.random() * (this.height/2 - 1000) + this.height /2 + 500);

        const redHand = new RedHand(randomX, randomY, this.collisionSystem);
        this.redHands.push(redHand);
      }
    }

    const lightbulb = new GameObject({
      id: 'lightbulb',
      x: lightbulbX,
      y: lightbulbY,
      z: 10,

      spriteSheets: {
        idle: './assets/sprites/lightbulb_dim.png'
      },

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
      id: 'mewo',
      x: rectX,
      y: rectY + 96,
      z: 0,

      spriteSheets: {
        idle: './assets/sprites/mewo_asleep.png'
      },

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
          group: 'solid',
          edges: [
            new Vector2([4, 4], [30, 4]),
            new Vector2([30, 4], [30, 16]),
            new Vector2([30, 16], [4, 16]),
            new Vector2([4, 16], [4, 4]),
          ]
        }
      ],

      triggers: [
        Trigger.newDownDialogue(),
        Trigger.newRightDialogue(),
        Trigger.newLeftDialogue(),
        Trigger.newUpDialogue(),
      ],
    });

    const laptop = new GameObject({
      id: 'laptop',
      x: rectX + 32,
      y: rectY,
      z: 0,
      
      spriteSheets: {
        idle: './assets/sprites/laptop_static.png'
      },

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
          group: 'solid',
          edges: [
            new Vector2([5, 4], [28, 4]),
            new Vector2([28, 4], [28, 20]),
            new Vector2([28, 20], [5, 20]),
            new Vector2([5, 20], [5, 4]),
          ]
        }
      ],

      triggers: [
        Trigger.newDownDialogue(),
        Trigger.newRightDialogue(),
        Trigger.newLeftDialogue(),
        Trigger.newUpDialogue(),
      ],
    });

    const sketchbook = new GameObject({
      id: 'sketchbook',
      x: rectX + 96,
      y: rectY,
      z: 0,
      spriteSheet: './assets/sprites/sketchbook.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'solid',
          edges: [
            new Vector2([5, 4], [28, 4]),
            new Vector2([28, 4], [28, 16]),
            new Vector2([28, 16], [5, 16]),
            new Vector2([5, 16], [5, 4]),
          ]
        }
      ],

      triggers: [
        Trigger.newDownDialogue(),
        Trigger.newRightDialogue(),
        Trigger.newLeftDialogue(),
        Trigger.newUpDialogue(),
      ],
    });

    const tissuebox = new GameObject({
      id: 'tissuebox',
      x: rectX + 96,
      y: rectY + 64,
      z: 0,
      spriteSheet: './assets/sprites/tissuebox.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'solid',
          edges: [
            new Vector2([5, 6], [28, 6]),
            new Vector2([28, 6], [28, 18]),
            new Vector2([28, 18], [5, 18]),
            new Vector2([5, 18], [5, 6]),
          ]
        }
      ],

      triggers: [
        Trigger.newDownDialogue(),
        Trigger.newRightDialogue(),
        Trigger.newLeftDialogue(),
        Trigger.newUpDialogue(),
      ],
    });

    const door = new GameObject({
      id: 'door',
      x: rectX,
      y: rectY - 96,
      z: 0,
      spriteSheet: './assets/sprites/door.png',

      collisionSystem: this.collisionSystem,
      colliders: [
        {
          group: 'solid',
          edges: [
            new Vector2([5, 0], [28, 0]),
            new Vector2([28, 0], [28, 22]),
            new Vector2([28, 22], [5, 22]),
            new Vector2([5, 22], [5, 0]),
          ]
        }
      ],

      triggers: [
        Trigger.newDownDialogue(),
        Trigger.newRightDialogue(),
        Trigger.newLeftDialogue(),
        Trigger.newUpDialogue(),
      ],
    });

    const lightbulbShadow = new GameObject({
      id: 'lighbulbShadow',
      x: lightbulb.x,
      y: lightbulb.y + 32,
      z: 0,
      spriteSheet: './assets/sprites/shadow_lightbulb.png',
    });

    const player = new Player(rectX + 64, rectY + 32, 5, this);
    player.lives = new Lives({
      x: canvas.width / 2 - 24,
      y: canvas.height / 2 - 32,
      scale: 2,
      spriteSheets: {
        '3lives': './assets/sprites/lives/lives_3.png',
        '2lives': './assets/sprites/lives/lives_2.png',
        '1lives': './assets/sprites/lives/lives_1.png',
        '0lives': './assets/sprites/lives/lives_0.png',
      }
    });

    this.objects = [
      this.rect = new Rect(rectX, rectY, -1, rectWidth, rectHeight),
      this.sketchbook = sketchbook,
      this.tissuebox = tissuebox,
      this.door = door,
      this.lightbulbShadow = lightbulbShadow,
      this.lightbulb = lightbulb,
      this.mewo = mewo,
      this.laptop = laptop,
      this.player = player,
      ...this.redHands
    ];

    this.objects.sort((a, b) => a.z - b.z);

    this.bgMusic = {
      url: 'assets/music/bg_white_space.mp3',
      loopEnd: 27.9
    };

    this.camera = new GameCamera(this.player, canvas.width, canvas.height, this.width, this.height, false);
    this.vignette = new Vignette(canvas.width, canvas.height, false, 0, 1);
    this.blackFlash = new Flash(canvas.width, canvas.height);
    this.endText = new TextFade();

    this.isGameOver = false;
    this.isGameWon = false;
    this.spawnSomething = false;
    this.isEndTextActive = false;
  }

  update(deltaTime) {
    console.log(this.blackFlash.isActive);
    this.camera.update(deltaTime);
    this.vignette.update(deltaTime);
    this.blackFlash.update(deltaTime);
    if (this.player.lives) this.player.lives.update(deltaTime);

    if (this.spawnSomething) {
      this.spawnSomething = false;
      const something = new Something(0, 0, this);
      something.follow(this.player);
      this.something = something;
      this.objects.push(this.something);
    }

    if (!this.isGameOver) {
      this.objects.forEach(obj => {
        obj.update(deltaTime);
      });
    } else {
      if (!this.isEndTextActive) {
        this.endText.configure("Te acoplaron", canvas.width / 2, canvas.height / 2).fadeIn(2);
      }
      this.endText.update(deltaTime);
      this.isEndTextActive = true;
    }
    
    this.#draw(deltaTime);

    // Camera wrapping logic
    const halfViewportWidth = this.camera.viewportWidth / 2;
    const halfViewportHeight = this.camera.viewportHeight / 2;

    // Left edge wrap
    if (this.camera.x <= -halfViewportWidth) {
        this.player.x += this.width;
        this.camera.x += this.width;
    } 
    // Right edge wrap
    else if (this.camera.x >= this.width - halfViewportWidth) {
        this.player.x -= this.width;
        this.camera.x -= this.width;
    }
    // Top edge wrap
    else if (this.camera.y <= -halfViewportHeight) {
        this.player.y += this.height;
        this.camera.y += this.height;
    }
    // Bottom edge wrap
    else if (this.camera.y >= this.height - halfViewportHeight) {
        this.player.y -= this.height;
        this.camera.y -= this.height;
    }

    this.camera.resetTransform(ctx);
  }

  #draw(deltaTime) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    this.camera.applyTransform(ctx);

    this.objects.forEach(obj => {
      obj.draw(ctx); 
    });

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y + 2);
    ctx.lineTo(this.lightbulb.x + 17, this.lightbulb.y - 128);
    ctx.strokeStyle = '#000000ff';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y - 128);
    ctx.lineTo(this.lightbulb.x + 17, this.lightbulb.y - 160);
    ctx.strokeStyle = '#000000cc';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y - 160);
    ctx.lineTo(this.lightbulb.x + 17, this.lightbulb.y - 192);
    ctx.strokeStyle = '#00000099';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y - 192);
    ctx.lineTo(this.lightbulb.x + 17, this.lightbulb.y - 224);
    ctx.strokeStyle = '#00000066';
    ctx.lineWidth = 1;
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y - 224);
    ctx.lineTo(this.lightbulb.x + 17, this.lightbulb.y - 256);
    ctx.strokeStyle = '#00000033';
    ctx.lineWidth = 1;
    ctx.stroke();

    this.camera.resetTransform(ctx);
    this.vignette.draw(ctx);
    this.blackFlash.draw(ctx);
    if (this.player.lives) {
      this.player.lives.draw(ctx);
    }
    this.endText.draw(ctx);
    super.draw(ctx, deltaTime);
  }

  adjustCanvas() {
    this.camera.viewportWidth = canvas.width;
    this.camera.viewportHeight = canvas.height;
    this.camera.offsetX = canvas.width / 2;
    this.camera.offsetY = canvas.height / 2;

    this.vignette.width = canvas.width;
    this.vignette.height = canvas.height;
    this.vignette.centerX = canvas.width / 2;
    this.vignette.centerY = canvas.height / 2;

    this.blackFlash.width = canvas.width;
    this.blackFlash.height = canvas.height;

    if (this.player.lives) {
      this.player.lives.x = canvas.width / 2 - 24;
      this.player.lives.y = canvas.height / 2 - 32;
    }
  }
}