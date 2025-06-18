export class TextFade {
  constructor() {
    this.text = '';
    this.x = 0;
    this.y = 0;
    this.alpha = 0;
    this.isActive = false;
    this.currentPhase = 'none'; // 'none', 'fadeIn', 'hold', 'fadeOut'
    this.fadeInDuration = 0;
    this.fadeOutDuration = 0;
    this.holdDuration = 0;
    this.elapsedTime = 0;
    this.maintainVisibility = false; // New flag to control persistence
    this.style = {
      font: '72px omori-blackspace',
      color: 'white',
      align: 'center',
      baseline: 'middle',
      shadowColor: 'black',
      shadowBlur: 0
    };
  }

  configure(text, x, y, style = {}) {
    this.text = text;
    this.x = x;
    this.y = y;
    this.style = { ...this.style, ...style };
    return this;
  }

  fadeIn(duration = 1, text, style) {
    if (text) this.text = text;
    if (style) this.style = { ...this.style, ...style };
    this.fadeInDuration = duration;
    this.fadeOutDuration = 0;
    this.holdDuration = 0;
    this.alpha = 0;
    this.elapsedTime = 0;
    this.currentPhase = 'fadeIn';
    this.isActive = true;
    this.maintainVisibility = true; // Will stay visible until fadeOut
    return this;
  }

  fadeOut(duration = 1) {
    if (this.alpha > 0) {
      this.fadeOutDuration = duration;
      this.elapsedTime = 0;
      this.currentPhase = 'fadeOut';
      this.isActive = true;
      this.maintainVisibility = false; // No longer maintaining visibility
    }
    return this;
  }

  flash(text, fadeInTime, fadeOutTime, holdTime = 0, style) {
    if (text) this.text = text;
    if (style) this.style = { ...this.style, ...style };
    this.fadeInDuration = fadeInTime;
    this.fadeOutDuration = fadeOutTime;
    this.holdDuration = holdTime;
    this.alpha = 0;
    this.elapsedTime = 0;
    this.currentPhase = 'fadeIn';
    this.isActive = true;
    this.maintainVisibility = false; // Flash will auto-fade out
    return this;
  }

  update(deltaTime) {
    if (!this.isActive) return;

    this.elapsedTime += deltaTime;

    switch (this.currentPhase) {
      case 'fadeIn':
        this.alpha = Math.min(1, this.elapsedTime / this.fadeInDuration);
        if (this.alpha >= 1) {
          this.elapsedTime = 0;
          // Only proceed to fadeOut if we're not maintaining visibility
          this.currentPhase = this.maintainVisibility ? 'hold' : 
                            (this.holdDuration > 0 ? 'hold' : 'fadeOut');
        }
        break;

      case 'hold':
        // For maintainVisibility, we stay in hold phase indefinitely
        if (!this.maintainVisibility && this.elapsedTime >= this.holdDuration) {
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
    // Draw if active OR if we're maintaining visibility (even if not "active")
    if ((!this.isActive && !this.maintainVisibility) || this.alpha <= 0) return;
    if (!this.text) return;

    ctx.save();
    ctx.font = this.style.font;
    ctx.fillStyle = this.style.color;
    ctx.textAlign = this.style.align;
    ctx.textBaseline = this.style.baseline;
    ctx.globalAlpha = this.alpha;
    
    if (this.style.shadowBlur > 0) {
      ctx.shadowColor = this.style.shadowColor;
      ctx.shadowBlur = this.style.shadowBlur;
    }
    
    ctx.fillText(this.text, this.x, this.y);
    ctx.restore();
  }

  setVisible(alpha) {
    this.alpha = Math.min(1, Math.max(0, alpha));
    this.isActive = alpha > 0;
    this.maintainVisibility = alpha > 0;
    this.currentPhase = 'hold';
    return this;
  }
}