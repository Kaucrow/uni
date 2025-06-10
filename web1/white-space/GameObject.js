import { Animator } from "./Animator.js";
import { Input } from "./Input.js";
import { Collider } from "./Collider.js";

export class GameObject {
  constructor(config) {
    // Base properties
    this.id = config.id || null;
    this.x = config.x || 0;
    this.y = config.y || 0;
    this.z = config.z || 0;
    this.oldX = this.x;
    this.oldY = this.y;
    this._preciseX = config.x || 0;
    this._preciseY = config.y || 0;
    this.width = config.width || 32;
    this.height = config.height || 32;

    // Action system
    this.actions = config.actions || {
      idle: { animates: false }
    };
    this.currentAction = config.defaultAction || 'idle';

    // Spritesheet
    this.spriteSheet = null;
    this.spriteSheets = null;
    if (config.spriteSheet) {
      this.spriteSheet = new Image();
      this.spriteSheet.src = config.spriteSheet;
    } else if (this.actions) {
      const spriteSheets = new Map(
        Object.entries(this.actions).map(([actionName, config]) => {
          const img = new Image();
          img.src = config.spriteSheet;
          return [actionName, img];
        })
      );

      this.spriteSheets = spriteSheets;
    }
    this.speed = config.speed || null;

    // Animator
    this.animator = config.animator ? new Animator(config.animator) : null;

    // Input
    this.input = config.input ? new Input(config.input) : null;

    // Collisions
    this.collisionSystem = config.collisionSystem || null;
    this.colliders = [];
    if (config.colliders) {
      config.colliders.forEach(collider => {
        const colliderObj = new Collider(collider, this);
        this.collisionSystem.register(colliderObj);
        this.colliders.push(colliderObj);
      });
    }
    this.collisionCallbacks = []; // When a collision triggers, the callbacks get stored here for execution

    // Custom properties
    this.customProperties = config.customProperties || {};
  }

  update(deltaTime) {
    if (this.input) {
      this.updateFromInput(deltaTime);
    }

    this.collisionCallbacks.forEach((callback) => callback(this));
    this.collisionCallbacks = [];

    if (this.actions[this.currentAction]?.animates) {
      if (this.animator) {
        this.animator.updateAnimation(deltaTime);
      } else {
        throw Error(`Action ${this.actions[this.currentAction]} is set to animate, but there's no animator object.`)
      }
    }
  }

  updateFromInput(deltaTime) {
    if (!this.input) return;

    const movement = this.input.getMovementVector();

    if (movement.y !== 0 || movement.x !== 0) {
      this.move(movement.x, movement.y, deltaTime);

      if (this.input.isKeyPressed('Shift')) {
        this.setAction('run');
      } else {
        this.setAction('walk');
      }

      if (this.animator) {
        const movementAnimation = this.input.getCurrentMovementAnimation(movement);
        this.animator.setAnimation(movementAnimation);
      }
    } else {
      this.setAction('idle');

      if (this.animator) {
        this.animator.frameX = this.animator.animations[this.animator.currentAnimation].defaultFrame;
      }
    }
  }

  draw(ctx) {
    let spriteSheet = null;

    if (this.spriteSheet) {
      spriteSheet = this.spriteSheet;
    } else {
      spriteSheet = this.spriteSheets.get(this.currentAction);
    };

    if (this.animator) {
      const animation = this.animator.animations[this.animator.currentAnimation];

      ctx.drawImage(
        spriteSheet,
        this.animator.frameX * this.width + this.animator.frameX,
        animation.y * this.height + animation.y,
        this.width - 1,
        this.height - 1,
        this.x,
        this.y,
        this.width,
        this.height
      );
    } else {
      ctx.drawImage(
        spriteSheet,
        0,
        0,
        this.width - 1,
        this.height - 1,
        this.x,
        this.y,
        this.width,
        this.height 
      )
    }

    if (this.colliders) {
      this.colliders.forEach((collider) => {
        if (collider.drawProps) { collider.draw(ctx) }
      })
    }
  }

  setAction(action) {
    if (this.actions[action]) {
      this.currentAction = action;
    }
  }

  move(dx, dy, deltaTime) {
    this.oldX = this.x;
    this.oldY = this.y;

    let speed = null;

    if (this.speed) {
      speed = this.speed;
    } else {
      if (!this.input) return;

      if (this.input.isKeyPressed('Shift')) {
        speed = this.input.runSpeed;
      } else {
        speed = this.input.walkSpeed;
      }
    }

    /*this._preciseX += dx * speed * deltaTime;
    this._preciseY += dy * speed * deltaTime;

    this.x = Math.round(this._preciseX);
    this.y = Math.round(this._preciseY);
    */

    this.x += Math.round(dx * speed * deltaTime);
    this.y += Math.round(dy * speed * deltaTime);

    this.collisionSystem.checkCollisions(this);
  }
}