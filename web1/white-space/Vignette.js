export class Vignette {
  constructor(width, height, flicker = false, intensity = 0.8, radius = 0.4) {
    this.height = height;
    this.width = width;
    this.centerX = this.width / 2;
    this.centerY = this.height / 2;
    this.flicker = flicker;
    
    // Current values
    this.intensity = intensity;
    this.radius = radius;
    
    // Target values for transitions
    this.targetIntensity = intensity;
    this.targetRadius = radius;
    
    // Transition control
    this.transitionSpeed = 0;
    this.isTransitioning = false;
    this.transitionCompleteCallback = null;

    // Flicker system
    this._flickerIntervalCounter = 0;
    this._flickerInterval = 0.1;
    this._flickerTimesCounter = 0;
    this._flickerLimit = 10;
    this._flickerCurrent = 'darken';
  }

  transitionTo(config, callback = null) {
    if (config.intensity !== undefined) {
      this.targetIntensity = config.intensity;
    }
    if (config.radius !== undefined) {
      this.targetRadius = config.radius;
    }

    const duration = config.duration || 1.0;
    this.transitionSpeed = 1.0 / duration;
    this.isTransitioning = true;
    this.transitionCompleteCallback = callback;
  }

  update(deltaTime) {
    // Handle transition animation
    if (this.isTransitioning) {
      const intensityDelta = this.targetIntensity - this.intensity;
      const radiusDelta = this.targetRadius - this.radius;
      
      const intensityStep = intensityDelta * this.transitionSpeed * deltaTime;
      const radiusStep = radiusDelta * this.transitionSpeed * deltaTime;
      
      this.intensity += intensityStep;
      this.radius += radiusStep;
      
      // Check if transition is complete
      if (Math.abs(intensityDelta) < 0.001 && Math.abs(radiusDelta) < 0.001) {
        this.intensity = this.targetIntensity;
        this.radius = this.targetRadius;
        this.isTransitioning = false;
        
        if (this.transitionCompleteCallback) {
          this.transitionCompleteCallback();
        }
      }
    }

    // Handle flicker effect (if enabled and not currently transitioning)
    if (this.flicker && !this.isTransitioning) {
      this._flickerIntervalCounter += deltaTime;

      if (this._flickerIntervalCounter >= this._flickerInterval) {
        this._flickerIntervalCounter = 0;

        if (this._flickerTimesCounter === this._flickerLimit) {
          this._flickerTimesCounter = 0;
          this._flickerCurrent = this._flickerCurrent === 'darken' ? 'lighten' : 'darken';
        }

        if (this._flickerCurrent === 'darken') 
          this.radius -= 0.005;
        else
          this.radius += 0.005;

        this._flickerTimesCounter++;
      }
    }
  }

  draw(ctx) {
    const gradient = ctx.createRadialGradient(
      this.centerX, this.centerY, this.height * this.radius,
      this.centerX, this.centerY, this.height * 0.2
    );

    gradient.addColorStop(0, `rgba(0, 0, 0, ${this.intensity})`);
    gradient.addColorStop(1, "rgba(0, 0, 0, 0)");

    ctx.fillStyle = gradient;
    ctx.globalCompositeOperation = "multiply";
    ctx.fillRect(0, 0, this.width, this.height);
    ctx.globalCompositeOperation = "source-over";
  }
}