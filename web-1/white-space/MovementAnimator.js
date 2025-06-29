export class MovementAnimator {
  constructor(gameObject, config) {
    this.gameObject = gameObject;
    this.getAnimationPrefix = () => '';
    if (config.getAnimationPrefix) {
      this.getAnimationPrefix = config.getAnimationPrefix;
    }
    this.directionMappings = config.directionMappings || {
      x: {
        1: 'Right',
        [-1]: 'Left'
      },
      y: {
        1: 'Down',
        [-1]: 'Up'
      }
    };
  }

  update(deltaTime) {
    const dx = this.gameObject.x - this.gameObject.oldX;
    const dy = this.gameObject.y - this.gameObject.oldY;

    if (dx !== 0 || dy !== 0) {
      this.updateAnimationFromMovement(dx, dy);
    }
  }

  updateAnimationFromMovement(dx, dy) {
    let directionX = 0;
    let directionY = 0;

    if (Math.abs(dx) > Math.abs(dy)) {
      directionX = Math.sign(dx);
    } else if (dy !== 0) {
      directionY = Math.sign(dy);
    }

    const xDir = this.directionMappings.x[directionX];
    const yDir = this.directionMappings.y[directionY];

    const direction = xDir || yDir;
    if (direction) {
      const animationPrefix = this.getAnimationPrefix();
      const animationName = animationPrefix ? `${animationPrefix}${direction}` : direction.toLowerCase();
      this.gameObject.animator.setAnimation(animationName);
    }
  }
}