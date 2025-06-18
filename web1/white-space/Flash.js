export class Flash {
  constructor(width, height) {
    this.width = width;
    this.height = height;
    this.alpha = 0;
    this.isActive = false;
    this.currentPhase = 'none'; // 'none', 'fadeIn', 'hold', 'fadeOut'
    this.fadeInDuration = 0;
    this.fadeOutDuration = 0;
    this.holdDuration = 0;
    this.elapsedTime = 0;
    this.maintainDarkness = false; // New flag to track if we should stay dark
  }

  flash(fadeInTime, fadeOutTime, holdTime = 0) {
    this.fadeInDuration = fadeInTime;
    this.fadeOutDuration = fadeOutTime;
    this.holdDuration = holdTime;
    this.alpha = 0;
    this.elapsedTime = 0;
    this.currentPhase = 'fadeIn';
    this.isActive = true;
    this.maintainDarkness = false; // Flash will auto-fade out
  }

  fadeIn(fadeInTime) {
    this.fadeInDuration = fadeInTime;
    this.fadeOutDuration = 0;
    this.holdDuration = 0;
    this.alpha = 0;
    this.elapsedTime = 0;
    this.currentPhase = 'fadeIn';
    this.isActive = true;
    this.maintainDarkness = true; // Will stay dark until fadeOut is called
  }

  fadeOut(fadeOutTime) {
    if (this.alpha > 0) {
      this.fadeOutDuration = fadeOutTime;
      this.elapsedTime = 0;
      this.currentPhase = 'fadeOut';
      this.isActive = true;
      this.maintainDarkness = false; // No longer maintaining darkness
    }
  }

  update(deltaTime) {
    if (!this.isActive) return;

    this.elapsedTime += deltaTime;

    switch (this.currentPhase) {
      case 'fadeIn':
        this.alpha = Math.min(1, this.elapsedTime / this.fadeInDuration);
        if (this.alpha >= 1) {
          this.elapsedTime = 0;
          // Only proceed to fadeOut if we're not maintaining darkness
          this.currentPhase = this.maintainDarkness ? 'hold' : 
                             (this.holdDuration > 0 ? 'hold' : 'fadeOut');
        }
        break;

      case 'hold':
        // For maintainDarkness, we stay in hold phase indefinitely
        if (!this.maintainDarkness && this.elapsedTime >= this.holdDuration) {
          this.elapsedTime = 0;
          this.currentPhase = 'fadeOut';
        }
        break;

      case 'fadeOut':
        this.alpha = Math.max(0, 1 - (this.elapsedTime / this.fadeOutDuration));
        if (this.alpha <= 0) {
          this.isActive = false;
          this.currentPhase = 'none';
        }
        break;
    }
  }

  draw(ctx) {
    // Draw if active OR if we're maintaining darkness (even if not "active")
    if ((!this.isActive && !this.maintainDarkness) || this.alpha <= 0) return;

    ctx.fillStyle = `rgba(0, 0, 0, ${this.alpha})`;
    ctx.globalCompositeOperation = "multiply"; // Darkens the underlying pixels
    ctx.fillRect(0, 0, this.width, this.height);
    ctx.globalCompositeOperation = "source-over"; // Reset to default
  }

  // Optional: Immediate darkness control
  setDarkness(alpha) {
    this.alpha = Math.min(1, Math.max(0, alpha));
    this.isActive = alpha > 0;
    this.maintainDarkness = alpha > 0;
    this.currentPhase = 'hold';
  }
}