import { ctx, canvas } from "./canvas.js";

export class Rect {
  constructor(x, y, z, width, height) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.width = width;
    this.height = height;
    this.borderWidth = 1;
  }

  update() {}

  draw() {
    ctx.fillStyle = 'rgba(0, 0, 0, 0)';
    ctx.strokeStyle = 'black';
    ctx.lineWidth = this.borderWidth;
    ctx.strokeRect(this.x, this.y, this.width, this.height);
  }
}