import { Player } from "./player.js";
import { Rect } from "./Rect.js";
import { Mewo } from "./mewo.js";
import { Lightbulb } from "./Lightbulb.js";
import { LightbulbShadow } from "./Shadow.js";
import { Laptop } from "./Laptop.js";
import { Sketchbook } from "./Sketchbook.js";
import { Tissuebox } from "./Tissuebox.js";
import { Door } from "./Door.js";
import { ctx, canvas } from "./canvas.js";

export class Room {
  constructor() {
    const rectWidth = 128;
    const rectHeight = 96;
    const rectX = canvas.width / 2 - rectWidth / 2;
    const rectY = canvas.height / 2 - rectHeight / 2;

    const lightbulbX = rectX + 64;
    const lightbulbY = rectY - 72;

    this.canvasWidth = canvas.width;
    this.canvasHeight = canvas.height;

    this.objects = [
      this.rect = new Rect(rectX, rectY, 128, 96),
      this.sketchbook = new Sketchbook(rectX + 96, rectY),
      this.tissuebox = new Tissuebox(rectX + 96, rectY + 64),
      this.door = new Door(rectX, rectY - 96),
      this.lightbulbShadow = new LightbulbShadow(lightbulbX, lightbulbY + 32)
    ];

    this.animatedObjects = [
      this.player = new Player(rectX + 64, rectY + 32),
      this.mewo = new Mewo(rectX, rectY + 96),
      this.lightbulb = new Lightbulb(lightbulbX, lightbulbY),
      this.laptop = new Laptop(rectX + 32, rectY),
    ];

    this.bgMusic = {
      url: 'assets/music/bg_white_space.mp3',
      loopEnd: 27.9
    };
  }

  update(deltaTime) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    this.rect.draw();

    this.lightbulbShadow.draw(ctx);

    this.door.draw(ctx);
  
    this.mewo.update(deltaTime);
    this.mewo.draw(ctx);

    this.laptop.update(deltaTime);
    this.laptop.draw(ctx);

    this.sketchbook.draw(ctx);

    this.tissuebox.draw(ctx);
  
    this.player.update(deltaTime);
    this.player.draw(ctx);

    this.lightbulb.update(deltaTime);
    this.lightbulb.draw(ctx);

    ctx.beginPath();
    ctx.moveTo(this.lightbulb.x + 17, this.lightbulb.y + 2);
    ctx.lineTo(this.lightbulb.x + 17, 0);
    ctx.strokeStyle = 'black';
    ctx.lineWidth = 1;
    ctx.stroke();
  }

  adjustCanvas() {
    let widthDiff = canvas.width - this.canvasWidth;
    let heightDiff = canvas.height - this.canvasHeight;

    for (const obj of this.objects) {
      obj.x += widthDiff / 2;
      obj.y += heightDiff / 2;
    }
    for (const obj of this.animatedObjects) {
      obj.x += widthDiff / 2;
      obj.y += heightDiff / 2;
    }

    this.canvasWidth = canvas.width;
    this.canvasHeight = canvas.height;
  }
}