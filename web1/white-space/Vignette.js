export class Vignette {
  constructor(width, height, flicker = false, intensity = 0.8, radius = 0.4) {
    this.height = height;
    this.width = width;
    this.centerX = width / 2;
    this.centerY = height / 2;
    this.flicker = flicker;
    this.intensity = intensity;
    this.radius = radius;

    this._flickerIntervalCounter = 0;
    this._flickerInterval = 0.1;
    this._flickerTimesCounter = 0;
    this._flickerLimit = 10;
    this._flickerCurrent = 'darken';
  }

  update(deltaTime) {
    if (!this.flicker) return;

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

  draw(ctx) {
    const gradient = ctx.createRadialGradient(
      this.centerX, this.centerY, this.height * this.radius,
      this.centerX, this.centerY, this.height * 0.2
    );

    gradient.addColorStop(0, `rgba(0, 0, 0, ${this.intensity})`);
    gradient.addColorStop(1, "rgba(0, 0, 0, 0)");

    ctx.fillStyle = gradient;
    ctx.globalCompositeOperation = "multiply"; // Darkens the underlying pixels
    ctx.fillRect(0, 0, this.width, this.height);
    ctx.globalCompositeOperation = "source-over"; // Reset to default
  }
}