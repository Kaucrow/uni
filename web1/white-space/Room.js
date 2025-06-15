import { CollisionSystem } from "./CollisionSystem.js"
import { DialogueBox } from "./DialogueBox.js";

export class Room {
  constructor(config) {
    this.collisionSystem = new CollisionSystem();
    this.width = config.width;
    this.height = config.height;
    this.camera = config.camera || undefined;
    this.dialogueBox = new DialogueBox();
    this.dialogueBox.isActive = true;
    this.dialogueBox.text = "el atla";
  }

  draw(ctx, deltaTime) {
    if (this.dialogueBox.isActive) {
      this.dialogueBox.draw(ctx, this.camera.viewportWidth, this.camera.viewportHeight);
    }
  }
}