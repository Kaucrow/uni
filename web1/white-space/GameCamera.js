export class GameCamera {
  constructor(target, viewportWidth, viewportHeight, roomWidth, roomHeight) {
    this.target = target; // The object to follow
    this.x = 0;
    this.y = 0;
    this.viewportWidth = viewportWidth;
    this.viewportHeight = viewportHeight;
    this.roomWidth = roomWidth;
    this.roomHeight = roomHeight;
    this.offsetX = viewportWidth / 2;
    this.offsetY = viewportHeight / 2;
  }

  update() {
    this.x = this.target.x - this.offsetX;
    this.y = this.target.y - this.offsetY;
  }

  applyTransform(ctx) {
    ctx.save();
    ctx.translate(-this.x, -this.y);
  }

  resetTransform(ctx) {
    ctx.restore();
  }
}