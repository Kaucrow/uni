import { Player } from "../Player.js";
import { RedHand } from "../RedHand.js";
import { Something } from "../Something.js";
import { Key } from "../Key.js";
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
import { RandomSpawner } from "../RandomSpawner.js";
import { audioPlayer } from "../Game.js";

export class WhiteSpace extends Room {
  constructor() {
    super({
      width: 3000,
      height: 3000,
      dialogues: {
        'mewo': [
          new Dialogue('Miau? (Esperando a que algo suceda?)', Dialogue.SPEED.NORMAL, () => {}),
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
              const keySpawner = new RandomSpawner(this.width, this.height);
              this.keys = keySpawner.spawnObjects(8, (x, y) => new Key(x, y, this));
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
    audioPlayer.load('victory', './assets/sfx/bgm/victory.mp3');
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

    const redHandSpawner = new RandomSpawner(this.width, this.height);
    this.redHands = redHandSpawner.spawnObjects(12, (x, y) => new RedHand(x, y, this.collisionSystem));

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
      this.mewo = mewo,
      this.laptop = laptop,
    ];

    this.objects.sort((a, b) => a.z - b.z);

    this.player = player;
    this.lightbulb = lightbulb,
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

    // Normal update
    if (!this.isGameOver) {
      this.objects.forEach(obj => {
        obj.update(deltaTime)
      });

      this.redHands.forEach(redHand => redHand.instance.update(deltaTime));

      if(this.keys) this.keys.forEach(keys => keys.instance.light.update(deltaTime));

      this.player.update(deltaTime);
      this.lightbulb.update(deltaTime);
    // If it's Game Over
    } else if (this.isGameOver) {
      if (!this.isEndTextActive) {
        this.endText.configure("Te acoplaron", canvas.width / 2, canvas.height / 2).fadeIn(2);
      }
      this.endText.update(deltaTime);
      this.isEndTextActive = true;
    // If it's Game Won
    } else {

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

    if (this.player.keysCollected < 8) {
      this.camera.applyTransform(ctx);

      this.objects.forEach(obj => obj.draw(ctx));

      this.player.draw(ctx);

      this.redHands.forEach(redHand => redHand.instance.draw(ctx));

      this.lightbulb.draw(ctx);

      this.camera.resetTransform(ctx);
      this.vignette.draw(ctx);
      this.camera.applyTransform(ctx);

      if (this.keys) {
        this.keys.forEach(light => light.instance.light.draw(ctx));
        this.keys.forEach(obj => obj.instance.draw(ctx));
      }

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

      this.blackFlash.draw(ctx);

      if (this.player.lives) {
        this.player.lives.draw(ctx);
      }
      this.endText.draw(ctx);
    } else {
      ctx.rect(0, 0, canvas.width, canvas.height);
      ctx.fillStyle = "white";
      ctx.fill();

      // Set text style
      ctx.fillStyle = "black";
      ctx.font = "48px omori-normal";
      ctx.textAlign = "center";
      ctx.textBaseline = "middle";

      // Calculate center position
      const centerX = canvas.width / 2;
      const centerY = canvas.height / 2;

      // Draw centered text
      ctx.fillText("Ganaste! :D (profe pongame 20 por favor esto tardo mucho tiempo)", centerX, centerY);
    }

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