import { GameObject } from "./GameObject.js";
import { Animator } from "./Animator.js";
import { CollisionSystem } from "./CollisionSystem.js";
import { Vector2 } from "./Vector.js";
import { KeyEvent } from "./KeyEvent.js";
import { audioPlayer } from "./Game.js";

export class Player extends GameObject {
  constructor(x, y, z, room) {
    super({
      id: 'player',
      tags: ['player'],
      x,
      y,
      z,
      width: 32,
      height: 32,
      room,

      spriteSheets: {
        idle: './assets/sprites/omori_walk.png',
        walk: './assets/sprites/omori_walk.png',
        run: './assets/sprites/omori_run.png',
      },

      actions: {
        idle: { animates: false },
        walk: { animates: true },
        run: { animates: true },
      },
      defaultAction: 'idle',

      animator: {
        animations: {
          walkDown: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkLeft: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 1,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkRight: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 2,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          walkUp: {
            frames: 3,
            frameDuration: 0.30,
            defaultFrame: 1,
            y: 3,
            direction: Animator.ANIMATION_DIRS.PINGPONG
          },
          runDown: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 0,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runLeft: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 1,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runRight: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 2,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
          runUp: {
            frames: 8,
            frameDuration: 0.10,
            defaultFrame: 1,
            y: 3,
            direction: Animator.ANIMATION_DIRS.LOOP
          },
        },
        animationDirection: 'forward',
      },
      movementAnimator: {
        getAnimationPrefix: () => {
          return this.currentAction === 'run' ? 'run' : 'walk'
        }
      },

      input: {
        walkSpeed: 96,
        runSpeed: 224,
      },

      collisionSystem: room.collisionSystem,
      colliders: [
        {
          draw: {},
          group: 'solid',
          interactions: [
            {
              group: 'solid',
              onCollide: (player) => CollisionSystem.FUNCS.STOP(player) 
            },
            {
              group: 'dialogue',
            },
            {
              group: 'key',
              onCollide: (player, key) => {
                player.keysCollected++;
                if (player.keysCollected >= 8) {
                  audioPlayer.stopAll();
                  audioPlayer.playOrQueue('victory');
                }
                key.Destroy();
              }
            },
          ],
          edges: [
            new Vector2([4, 2], [28, 2]),
            new Vector2([28, 2], [28, 32]),
            new Vector2([28, 32], [4, 32]),
            new Vector2([4, 32], [4, 2]),
          ],
        },
        {
          group: 'player',
          interactions: [
            {
              group: 'something',
              onCollide: (source) => {
              }
            }
          ],
          edges: [
            new Vector2([4, 2], [28, 2]),
            new Vector2([28, 2], [28, 32]),
            new Vector2([28, 32], [4, 32]),
            new Vector2([4, 32], [4, 2]),
          ],
        }
      ],
    })

    this.input.addOnKeyDown(new KeyEvent('z', (player) => {
      let foundDialogueTrigger = false;
      for (const trigger of player.inTriggers) {
        if (foundDialogueTrigger === true) continue;

        if (trigger.group === 'dialogue') {
          foundDialogueTrigger = true;

          if (!player.room.dialogueBox.isActive)
            player.room.dialogueBox.enable(trigger.parent);
          else if (player.room.dialogueBox.completed)
            player.room.dialogueBox.advanceDialogue();
          else
            player.room.dialogueBox.speedUpDialogue();
        }
      }
    }));

    this.keysCollected = 0;
  }

  update(deltaTime) {
    super.update(deltaTime);
  }
}