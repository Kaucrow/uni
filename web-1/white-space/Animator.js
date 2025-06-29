export class Animator {
  static ANIMATION_DIRS = {
    LOOP: 'loop',
    PINGPONG: 'pingpong'
  };

  constructor(config) {
    this.animations = config.animations || {};
    this.currentAnimation = config.defaultAnimation || Object.keys(this.animations)[0];
    this.frameX = this.animations[this.currentAnimation].defaultFrame || 0;
    this._frameDuration = config.frameDuration || null; // Seconds per frame
    this.animationTimer = 0;
    this.animationDirection = 'forward';
  }

  get frameDuration() {
    if (this._frameDuration) return this._frameDuration;

    return this.animations[this.currentAnimation].frameDuration;
  }

  set frameDuration(val) {
    this._frameDuration = val;
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

    if (animation.direction === Animator.ANIMATION_DIRS.PINGPONG) {
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

  setAnimation(name) {
    if (this.animations[name] && this.currentAnimation !== name) {
      this.currentAnimation = name;
      this.frameX = 0;
      this.animationDirection = 'forward';
    }
  }
}