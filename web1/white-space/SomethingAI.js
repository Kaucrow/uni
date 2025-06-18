import { GameObject } from "./GameObject.js";

export class SomethingAI {
  static STATES = {
    IDLE: 'idle',
    FOLLOW: 'follow',
  };

  constructor(something) {
    this.something = something;
    this.followObj = null;
    this.state = SomethingAI.STATES.IDLE;
  }

  update(deltaTime) {
    switch (this.state) {
      case SomethingAI.STATES.IDLE: break;

      case SomethingAI.STATES.FOLLOW: {
        if (!this.followObj) {
          this.state = SomethingAI.STATES.IDLE;
          break;
        }

        // Calculate direction vector
        const dx = this.followObj.x - this.something.x;
        const dy = this.followObj.y - this.something.y;
        
        // Calculate distance
        const distance = Math.sqrt(dx * dx + dy * dy);
        
        // Normalize the direction vector (convert to unit vector)
        if (distance > 0) {
          const vx = dx / distance;
          const vy = dy / distance;
          
          this.something.speed = Math.max(distance * 1.5, 150);

          if (this.something.room.vignette) {
            this.something.room.vignette.radius = Math.max(distance * 0.005, 0.4);
            this.something.room.vignette.intensity = Math.min(distance * 0.05, 1);
          }

          // Move towards the target
          this.something.move(vx, vy, deltaTime);
          //this.something.x += vx * this.something.speed;
          //this.something.y += vy * this.something.speed;
        }

        // Optional: face the direction of movement
        if (dx !== 0 || dy !== 0) {
          this.something.rotation = Math.atan2(dy, dx);
        }
      }
    }
  }

  follow(object) {
    if (!(object instanceof GameObject)) throw new Error("Object to follow must be a GameObject");

    this.followObj = object;
    this.state = SomethingAI.STATES.FOLLOW;
  }
}