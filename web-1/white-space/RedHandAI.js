export class RedHandAI {
  static STATES = {
    IDLE: 'idle',
    MOVING: 'moving'
  };

  constructor(redHand, config) {
    this.redHand = redHand;
    this.movementTimer = 0;
    this.movementInterval = 1; // Change direction every 1000ms (1 second)
    this.idleTimer = 0;
    this.idleInterval = 1;
    this.possibleDirections = ['down', 'left', 'right', 'up'];
    const randomIndex = Math.floor(Math.random() * this.possibleDirections.length);
    this.direction = this.possibleDirections[randomIndex];
    this.state = RedHandAI.STATES.MOVING;
  }

  update(deltaTime) {
    // Update the direction change timer
    if (this.state === RedHandAI.STATES.IDLE) {
      this.idleTimer += deltaTime;
      if (this.idleTimer >= this.idleInterval) {
        this.idleTimer = 0;
        this.state = RedHandAI.STATES.MOVING;
        this.changeDirection();
      }
    } else if (this.state === RedHandAI.STATES.MOVING) {
      this.movementTimer += deltaTime;

      if (this.movementTimer >= this.movementInterval) {
        this.movementTimer = 0;
        this.state = RedHandAI.STATES.IDLE;
      }
      
      // Move in the current direction
      switch (this.direction) {
        case 'down': this.redHand.move(0, 1, deltaTime); break;
        case 'left': this.redHand.move(-1, 0, deltaTime); break;
        case 'right': this.redHand.move(1, 0, deltaTime); break;
        case 'up': this.redHand.move(0, -1, deltaTime); break;
      }
    }
  }

  changeDirection() {
    // Filter out the current direction to avoid immediate repeats
    const availableDirections = this.possibleDirections.filter(dir => dir !== this.direction);
    
    // Randomly select a new direction from available ones
    const randomIndex = Math.floor(Math.random() * availableDirections.length);
    this.direction = availableDirections[randomIndex];
  }
}