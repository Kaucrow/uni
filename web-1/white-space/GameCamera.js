export class GameCamera {
  constructor(target, viewportWidth, viewportHeight, roomWidth, roomHeight, shouldClamp = true) {
    this.target = target;
    this.x = 0;
    this.y = 0;
    this.targetX = 0; // Actual camera position we're moving toward
    this.targetY = 0;
    this.viewportWidth = viewportWidth;
    this.viewportHeight = viewportHeight;
    this.roomWidth = roomWidth;
    this.roomHeight = roomHeight;
    this.offsetX = viewportWidth / 2;
    this.offsetY = viewportHeight / 2;
    this.dpr = window.devicePixelRatio || 1;
    this.smoothness = 8; // Higher = slower smoothing (typical range 5-15)
    this.shouldClamp = shouldClamp;
  }

  update(deltaTime) {
    // Calculate target position (in physical pixels)
    const targetPixelX = this.target.x * this.dpr;
    const targetPixelY = this.target.y * this.dpr;
    
    // Determine where camera should move to
    this.targetX = Math.round(targetPixelX - this.offsetX * this.dpr) / this.dpr;
    this.targetY = Math.round(targetPixelY - this.offsetY * this.dpr) / this.dpr;
    
    // Clamp to room boundaries
    if (this.shouldClamp) {
      this.targetX = Math.max(0, Math.min(this.targetX, this.roomWidth - this.viewportWidth));
      this.targetY = Math.max(0, Math.min(this.targetY, this.roomHeight - this.viewportHeight));
    }
    
    // Smooth interpolation using deltaTime
    const smoothFactor = 1 - Math.exp(-this.smoothness * deltaTime);
    this.x = lerp(this.x, this.targetX, smoothFactor);
    this.y = lerp(this.y, this.targetY, smoothFactor);
    
    // Round to prevent sub-pixel shaking
    this.x = Math.round(this.x * this.dpr) / this.dpr;
    this.y = Math.round(this.y * this.dpr) / this.dpr;
  }

  applyTransform(ctx) {
    ctx.save();
    // Apply with half-pixel offset for crisp rendering
    ctx.translate(
      -Math.floor(this.x * this.dpr) / this.dpr + 0.5,
      -Math.floor(this.y * this.dpr) / this.dpr + 0.5
    );
  }

  resetTransform(ctx) {
    ctx.restore();
  }
}

// Linear interpolation helper
function lerp(start, end, t) {
  return start * (1 - t) + end * t;
}