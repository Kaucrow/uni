export class Input {
  constructor(object, config) {
    this.object = object;

    this.keys = {
      ArrowDown: false,
      ArrowLeft: false,
      ArrowRight: false,
      ArrowUp: false,
      Shift: false,
      z: false,
    };

    this.walkSpeed = config.walkSpeed || null;
    this.runSpeed = config.runSpeed || null;

    this.cardinalMovement = config.cardinalMovement || false;
    this.lastHorizontalKeyPress = 0;
    this.lastVerticalKeyPress = 0;

    this.onKeyUp = [];
    this.onKeyDown = [];
    this.onKeyPress = [];

    this.#setupEventListeners();
  }

  addOnKeyDown(event) {
    this.onKeyDown.push(event);
  }

  addOnKeyUp(event) {
    this.onKeyUp.push(event);
  }

  handleKeyDown(e) {
    if (this.keys.hasOwnProperty(e.key)) {
      const now = Date.now();
      if (['ArrowLeft', 'ArrowRight'].includes(e.key)) {
        this.lastHorizontalKeyPress = now;
      } else if (['ArrowUp', 'ArrowDown'].includes(e.key)) {
        this.lastVerticalKeyPress = now;
      }

      const keyPressed = this.keys[e.key];

      if (!keyPressed) {
        this.keys[e.key] = true;
        this.onKeyDown.forEach(event => {
          if (event.key === e.key && event.enabled) {
            event.callback(this.object);
          }
        })
      }
    }
  }

  handleKeyUp(e) {
    if (this.keys.hasOwnProperty(e.key)) {
      this.keys[e.key] = false;
      this.onKeyUp.forEach(event => {
        if (event.key === e.key && event.enabled) {
          event.callback(this.object);
        }
      })
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
      const x = (this.isKeyPressed('ArrowRight') ? 1 : 0) - (this.isKeyPressed('ArrowLeft') ? 1 : 0);
      const y = (this.isKeyPressed('ArrowDown') ? 1 : 0) - (this.isKeyPressed('ArrowUp') ? 1 : 0);

      if (x !== 0 && y !== 0) {
        if (this.lastHorizontalKeyPress > this.lastVerticalKeyPress) {
          return { x: x, y: 0 };
        } else {
          return { x: 0, y: y };
        }
      }

      return {
        x: x, y: y
      };

      /*
      return {
        x: (this.isKeyPressed('ArrowRight') ? 1 : 0) - (this.isKeyPressed('ArrowLeft') ? 1 : 0),
        y: (this.isKeyPressed('ArrowDown') ? 1 : 0) - (this.isKeyPressed('ArrowUp') ? 1 : 0)
      };*/
    }
  }

  #setupEventListeners() {
    document.addEventListener('keydown', (e) => this.handleKeyDown(e));
    document.addEventListener('keyup', (e) => this.handleKeyUp(e));
  }
}