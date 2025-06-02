import { Animator } from "./Animator.js";
import { Input } from "./Input.js";
import { Collider } from "./Collider.js";

export class GameObject {
  constructor(config) {
    // Base properties
    this.x = config.x || 0;
    this.y = config.y || 0;
    this.z = config.z || 0;
    this.oldX = this.x;
    this.oldY = this.y;
    this._preciseX = this.x;
    this._preciseY = this.y;
    this.width = config.width || 32;
    this.height = config.height || 32;
    this.spriteSheet = null;
    if (config.spriteSheet) {
      this.spriteSheet = new Image();
      this.spriteSheet.src = config.spriteSheet;
    }
    this.speed = config.speed || 50; // Pixels per second

    // Action system
    this.actions = config.actions || {
      idle: { animates: false }
    };
    this.currentAction = config.defaultAction || 'idle';

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

    // Custom properties
    this.customProperties = config.customProperties || {};
  }

  update(deltaTime) {
    if (this.input) {
      this.updateFromInput(deltaTime);
    }

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
      this.setAction('walk');

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
    if (!this.spriteSheet) return;

    if (this.animator) {
      const animation = this.animator.animations[this.animator.currentAnimation];

      ctx.drawImage(
        this.spriteSheet,
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
        this.spriteSheet,
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

    /*
    this._preciseX += dx * this.speed * deltaTime;
    this._preciseY += dy * this.speed * deltaTime;

    this.x = Math.floor(this._preciseX + 0.5);
    this.y = Math.floor(this._preciseY + 0.5);
    */

    this.x += Math.round(dx * this.speed * deltaTime);
    this.y += Math.round(dy * this.speed * deltaTime);

    this.collisionSystem.checkCollisions(this);
  }
}