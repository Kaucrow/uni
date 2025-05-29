export class GameObject {
  static ANIMATION_DIRS = {
    LOOP: 'loop',
    PINGPONG: 'pingpong'
  };

  constructor(config) {
    // Base properties
    this.x = config.x || 0;
    this.y = config.y || 0;
    this._preciseX = this.x;
    this._preciseY = this.y;
    this.width = config.width || 32;
    this.height = config.height || 32;
    this.spriteSheet = config.spriteSheet || null;

    // Action system
    this.actions = config.actions || {
      idle: { animates: false }
    };
    this.currentAction = config.defaultAction || 'idle';

    // Animation properties
    this.animations = config.animations || {};
    this.currentAnimation = config.defaultAnimation || Object.keys(this.animations)[0];
    this.frameX = this.animations[this.currentAnimation].defaultFrame || 0;
    this.frameDuration = config.frameDuration || 0.1; // Seconds per frame
    this.animationTimer = 0;
    this.animationDirection = 'forward';

    // Movement properties
    this.speed = config.speed || 50; // Pixels per second

    // Controller
    this.controller = config.controller || null;

    // Custom properties
    this.customProperties = config.customProperties || {};
  }

  update(deltaTime) {
    if (this.actions[this.currentAction]?.animates) {
      this.updateAnimation(deltaTime);
    }
  }

  updateFromController(deltatime) {
    if (!this.controller) return;

    const movement = this.controller.getMovementVector();
    const movementAnimation = this.controller.getCurrentMovementAnimation(movement);

    if (movementAnimation) {
      this.move(movement.x, movement.y, deltatime);
      this.setAction('walk');
      this.setAnimation(movementAnimation);
    } else {
      this.frameX = this.animations[this.currentAnimation].defaultFrame;
      this.setAction('idle');
    }
  }

  updateAnimation(deltaTime) {
    this.animationTimer += deltaTime;
    if (this.animationTimer >= this.frameDuration) {
      this.animationTimer -= this.frameDuration; // Preserve leftover time
      this.advanceFrame();
    }
  }

  advanceFrame() {
    const animation = this.animations[this.currentAnimation];

    if (animation.direction === GameObject.ANIMATION_DIRS.PINGPONG) {
      if (this.animationDirection === 'forward') {
        this.frameX++;
        if (this.frameX >= animation.frames - 1) {
          this.animationDirection = 'backward';
        }
      } else if (this.animationDirection === 'backward') {
        this.frameX--;
        if (this.frameX <= 0) {
          this.animationDirection = 'forward';
        }
      }
    } else { // LOOP by default
      this.frameX = (this.frameX + 1) % animation.frames;
    }
  }

  draw(ctx) {
    if (!this.spriteSheet) return;

    const animation = this.animations[this.currentAnimation];

    ctx.drawImage(
      this.spriteSheet,
      this.frameX * this.width + this.frameX,
      animation.y * this.height + animation.y,
      this.width - 1,
      this.height - 1,
      this.x,
      this.y,
      this.width,
      this.height
    );
  }

  setAnimation(name) {
    if (this.animations[name] && this.currentAnimation !== name) {
      this.currentAnimation = name;
      this.frameX = 0;
      this.animationDirection = 'forward';
    }
  }

  setAction(action) {
    if (this.actions[action]) {
      this.currentAction = action;
    }
  }

  move(dx, dy, deltaTime) {
    /*
    this._preciseX += dx * this.speed * deltaTime;
    this._preciseY += dy * this.speed * deltaTime;

    this.x = Math.floor(this._preciseX + 0.5);
    this.y = Math.floor(this._preciseY + 0.5);
    */

    this.x += Math.round(dx * this.speed * deltaTime);
    this.y += Math.round(dy * this.speed * deltaTime);
  }
}