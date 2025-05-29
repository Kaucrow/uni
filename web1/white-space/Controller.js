export class Controller {
  constructor(config) {
    this.keys = {
      ArrowDown: false,
      ArrowLeft: false,
      ArrowRight: false,
      ArrowUp: false
    };

    this.animationMappings = config.animationMappings || null;

    this.cardinalMovement = config.cardinalMovement || true;
    this.lastHorizontalKeyPress = 0;
    this.lastVerticalKeyPress = 0;

    this.setupEventListeners();
  }

  setupEventListeners() {
    document.addEventListener('keydown', (e) => this.handleKeyDown(e));
    document.addEventListener('keyup', (e) => this.handleKeyUp(e));
  }

  handleKeyDown(e) {
    if (this.keys.hasOwnProperty(e.key)) {
      const now = Date.now();
      if (['ArrowLeft', 'ArrowRight'].includes(e.key)) {
        this.lastHorizontalKeyPress = now;
      } else if (['ArrowUp', 'ArrowDown'].includes(e.key)) {
        this.lastVerticalKeyPress = now;
      }

      this.keys[e.key] = true;
    }
  }

  handleKeyUp(e) {
    if (this.keys.hasOwnProperty(e.key)) {
      this.keys[e.key] = false;
    }
  }

  isKeyPressed(key) {
    return this.keys[key] || false;
  }

  getMovementVector() {
    if (this.cardinalMovement) {
      // Cardinal movement logic (prioritizes last key pressed)
      const x = (this.isKeyPressed('ArrowRight') ? 1 : 0) - (this.isKeyPressed('ArrowLeft') ? 1 : 0);
      const y = (this.isKeyPressed('ArrowDown') ? 1 : 0) - (this.isKeyPressed('ArrowUp') ? 1 : 0);

      // Only allow movement along one axis at a time
      if (x !== 0 || y !== 0) {
        // Prioritize the most recently pressed key
        if (this.lastHorizontalKeyPress > this.lastVerticalKeyPress) {
          return { x: x, y: 0 };
        } else {
          return { x: 0, y: y };
        }
      } else {
        return { x: 0, y: 0};
      }
    } else {
      return {
        x: (this.isKeyPressed('ArrowRight') ? 1 : 0) - (this.isKeyPressed('ArrowLeft') ? 1 : 0),
        y: (this.isKeyPressed('ArrowDown') ? 1 : 0) - (this.isKeyPressed('ArrowUp') ? 1 : 0)
      };
    }
  }

  getCurrentMovementAnimation(movement) {
    if (movement.x !== 0) {
      return this.animationMappings.movement.x[movement.x];
    }
    if (movement.y !== 0) {
      return this.animationMappings.movement.y[movement.y];
    }
    return null;
  }
}