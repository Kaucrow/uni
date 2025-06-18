import { CollisionSystem } from "./CollisionSystem.js";
import { GameObject } from "./GameObject.js";
import { SomethingAI } from "./SomethingAI.js";
import { Vector2 } from "./Vector.js";

export class Something extends GameObject {
  constructor(x, y, room) {
    super({
      id: 'something',
      x,
      y,
      z: 5,
      width: 32,
      height: 64,
      spriteSheet: './assets/sprites/something_detailed.png',
      speed: 0,
      room,

      actions: {
        idle: { animates: false }
      },
      defaultAction: 'idle',

      collisionSystem: room.collisionSystem,
      colliders: [
        {
          group: 'something',
          interactions: [
            {
              group: 'player',
              onCollide: async (something, player) => {
                player.room.isCutsceneActive = true;
                CollisionSystem.FUNCS.STOP(player);
                
                const wait = (ms) => new Promise(resolve => setTimeout(resolve, ms));

                try {
                  player.room.whiteFlash.fadeIn(1);
                  await wait(2000);

                  something.x = 0;
                  if (player.room.rect) {
                    player.x = player.room.rect.x + 64;
                    player.y = player.room.rect.y + 32;
                  }

                  // Calculate direction vector
                  const dx = player.x - something.x;
                  const dy = player.y - something.y;
        
                  // Calculate distance
                  const distance = Math.sqrt(dx * dx + dy * dy);
                  if (something.room.vignette) {
                    something.room.vignette.radius = Math.max(distance * 0.005, 0.4);
                    something.room.vignette.intensity = Math.min(distance * 0.05, 1);
                  }
                  player.lives.fadeIn(2);
                  await wait(2000);

                  player.lives.decrement();
                  player.lives.decrement();
                  player.lives.decrement();
                  await wait(1000);

                  player.lives.fadeOut(2);
                  await wait(2000);

                  player.room.isGameOver = player.lives.isGameOver();

                  if (!player.room.isGameOver) {
                    player.room.whiteFlash.fadeOut(1);
                    await wait (1000);
                  }
                } finally {
                  player.room.isCutsceneActive = false;
                }
              }
            }
          ],
          edges: [
            new Vector2([4, 0], [28, 0]),
            new Vector2([28, 0], [28, 64]),
            new Vector2([28, 64], [4, 64]),
            new Vector2([4, 64], [4, 0]),
          ]
        }
      ]
    });

    this.AI = new SomethingAI(this);
  }

  update(deltaTime) {
    if (!this.room.isCutsceneActive) {
      this.AI.update(deltaTime);
      super.update(deltaTime);
    }
  }

  follow(object) {
    this.AI.follow(object);
  }
}