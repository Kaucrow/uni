export class LightSource {
  constructor(x, y, radius = 100, color = '#FFEECC', flickerConfig = null) {
    this.x = x;
    this.y = y;
    this.baseRadius = radius;
    this.baseColor = this._normalizeHex(color);
    
    // Current values
    this.radius = radius;
    this.color = this.baseColor;
    this.intensity = 0.8;
    
    // Target values
    this.targetRadius = radius;
    this.targetColor = this.baseColor;
    this.targetIntensity = this.intensity;
    
    // Transition control
    this.transitionSpeed = 0;
    this.isTransitioning = false;
    this.transitionCompleteCallback = null;

    // Enhanced Flicker system
    this.flicker = flickerConfig ? {
      enabled: true,
      low: flickerConfig.low || 0.8,    // 80% of base radius
      high: flickerConfig.high || 1.2,  // 120% of base radius
      speed: flickerConfig.speed || 1.0,
      phase: 0
    } : null;

    // Color storage
    this._currentHex = this.baseColor;
    this._currentRgb = this._hexToRgb(this.baseColor);
  }

  // Helper methods for color conversion
  _normalizeHex(hex) {
    // Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
    return hex.replace(/^#?([a-f\d])([a-f\d])([a-f\d])$/i, 
             (m, r, g, b) => `#${r}${r}${g}${g}${b}${b}`);
  }

  _hexToRgb(hex) {
    hex = this._normalizeHex(hex);
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
      r: parseInt(result[1], 16),
      g: parseInt(result[2], 16),
      b: parseInt(result[3], 16)
    } : { r: 255, g: 238, b: 204 }; // Default warm white
  }

  _rgbToHex(r, g, b) {
    return `#${[r, g, b].map(x => {
      const hex = x.toString(16);
      return hex.length === 1 ? `0${hex}` : hex;
    }).join('')}`;
  }

  // Linear interpolation helper
  _lerp(a, b, t) {
    return a + (b - a) * t;
  }

  // Set new position
  setPosition(x, y) {
    this.x = x;
    this.y = y;
  }

  transitionTo(config, callback = null) {
    if (config.radius !== undefined) {
      this.targetRadius = config.radius;
    }
    if (config.color !== undefined) {
      this.targetColor = this._normalizeHex(config.color);
      this._currentRgb = this._hexToRgb(this.targetColor);
    }
    if (config.intensity !== undefined) {
      this.targetIntensity = config.intensity;
    }

    const duration = config.duration || 1.0;
    this.transitionSpeed = 1.0 / duration;
    this.isTransitioning = true;
    this.transitionCompleteCallback = callback;
  }

  update(deltaTime) {
    // Handle transition animation
    if (this.isTransitioning) {
      const radiusDelta = this.targetRadius - this.radius;
      const intensityDelta = this.targetIntensity - this.intensity;
      
      this.radius += radiusDelta * this.transitionSpeed * deltaTime;
      this.intensity += intensityDelta * this.transitionSpeed * deltaTime;
      
      // Update current color
      this.color = this._rgbToHex(
        this._currentRgb.r,
        this._currentRgb.g,
        this._currentRgb.b
      );
      
      // Check transition completion
      if (Math.abs(radiusDelta) < 0.1 && Math.abs(intensityDelta) < 0.001) {
        this.radius = this.targetRadius;
        this.intensity = this.targetIntensity;
        this.isTransitioning = false;
        if (this.transitionCompleteCallback) this.transitionCompleteCallback();
      }
    }

    // Handle flicker
    if (this.flicker?.enabled && !this.isTransitioning) {
      // Update phase (looping between 0 and 1)
      this.flicker.phase += deltaTime * this.flicker.speed;
      this.flicker.phase %= 1;

      // Calculate current radius using sine wave for smooth oscillation
      const t = Math.sin(this.flicker.phase * Math.PI * 2) * 0.5 + 0.5;
      this.radius = this._lerp(
        this.baseRadius * this.flicker.low,
        this.baseRadius * this.flicker.high,
        t
      );
    }
  }

  draw(ctx) {
    const gradient = ctx.createRadialGradient(
      this.x, this.y, this.radius * 0.2,
      this.x, this.y, this.radius
    );

    gradient.addColorStop(0, `${this.color}${Math.floor(this.intensity * 255).toString(16).padStart(2, '0')}`);
    gradient.addColorStop(1, `${this.color}00`);

    ctx.fillStyle = gradient;
    ctx.globalCompositeOperation = "lighter";
    ctx.beginPath();
    ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
    ctx.fill();
    ctx.globalCompositeOperation = "source-over";
  }
}