export class Lives {
  constructor(config) {
    this.x = config.x;
    this.y = config.y; 
    this.scale = config.scale || 1;
    this.currentLives = 3;
    this.alpha = 0; // Default: fully invisible
    this.fadeState = 'none'; // 'none', 'fadeIn', 'fadeOut'
    this.fadeDuration = 0;
    this.fadeElapsed = 0;

    this.spriteSheets = {
      '3lives': new Image(),
      '2lives': new Image(),
      '1lives': new Image(),
      '0lives': new Image(),
    };

    // Load all sprite sheets
    Object.keys(config.spriteSheets).forEach(key => {
      this.spriteSheets[key].src = config.spriteSheets[key];
    });

    // Track loaded state
    this.loaded = false;
    this.imagesLoaded = 0;
    const totalImages = Object.keys(this.spriteSheets).length;

    // Set up load handlers
    Object.values(this.spriteSheets).forEach(img => {
      img.onload = () => {
        this.imagesLoaded++;
        if (this.imagesLoaded === totalImages) {
          this.loaded = true;
        }
      };
      img.onerror = () => console.error(`Failed to load lives image: ${img.src}`);
    });
  }

  update(deltaTime) {
    if (this.fadeState === 'none') return;

    this.fadeElapsed += deltaTime;
    const progress = Math.min(1, this.fadeElapsed / this.fadeDuration);

    switch (this.fadeState) {
      case 'fadeIn':
        this.alpha = progress;
        if (progress >= 1) {
          this.fadeState = 'none';
        }
        break;

      case 'fadeOut':
        this.alpha = 1 - progress;
        if (progress >= 1) {
          this.fadeState = 'none';
        }
        break;
    }
  }

  fadeIn(duration = 1) {
    this.fadeState = 'fadeIn';
    this.fadeDuration = duration;
    this.fadeElapsed = 0;
    this.alpha = 0; // Start from transparent
  }

  fadeOut(duration = 1) {
    this.fadeState = 'fadeOut';
    this.fadeDuration = duration;
    this.fadeElapsed = 0;
    this.alpha = 1; // Start from fully visible
  }

  draw(ctx, lives = this.currentLives) {
    if (!this.loaded || this.alpha <= 0) return;
    
    this.currentLives = Math.max(0, Math.min(3, lives));
    const spriteKey = `${this.currentLives}lives`;
    const sprite = this.spriteSheets[spriteKey];

    if (!sprite) return;

    ctx.save();
    
    // Apply scaling if needed
    if (this.scale !== 1) {
      ctx.scale(this.scale, this.scale);
      const scaledX = this.x / this.scale;
      const scaledY = this.y / this.scale;
      ctx.globalAlpha = this.alpha;
      ctx.drawImage(sprite, scaledX, scaledY);
    } else {
      ctx.globalAlpha = this.alpha;
      ctx.drawImage(sprite, this.x, this.y);
    }
    
    ctx.restore();
  }

  // Modified helper methods with optional fading
  decrement(fadeDuration = 0) {
    this.currentLives = Math.max(0, this.currentLives - 1);
    if (fadeDuration > 0) {
      this.fadeOut(fadeDuration);
      setTimeout(() => this.fadeIn(fadeDuration), fadeDuration * 1000);
    }
    return this.currentLives;
  }

  increment(fadeDuration = 0) {
    this.currentLives = Math.min(3, this.currentLives + 1);
    if (fadeDuration > 0) {
      this.fadeOut(fadeDuration);
      setTimeout(() => this.fadeIn(fadeDuration), fadeDuration * 1000);
    }
    return this.currentLives;
  }

  reset(fadeDuration = 0) {
    this.currentLives = 3;
    if (fadeDuration > 0) {
      this.fadeIn(fadeDuration);
    }
  }

  isGameOver() {
    return this.currentLives <= 0;
  }
}