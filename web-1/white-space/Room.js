import { CollisionSystem } from "./CollisionSystem.js"
import { DialogueBox } from "./DialogueBox.js";

export class Room {
  constructor(config) {
    this.collisionSystem = new CollisionSystem();
    this.width = config.width;
    this.height = config.height;
    this.camera = config.camera || undefined;
    // TODO: this.lightSystem = new LightSystem();
    this.dialogueBox = new DialogueBox(this);
    this.dialogues = config.dialogues || null;
    this.isCutsceneActive = false;
  }

  draw(ctx, deltaTime) {
    if (this.dialogueBox.isActive) {
      this.dialogueBox.update(deltaTime);
      this.dialogueBox.draw(ctx, this.camera.viewportWidth, this.camera.viewportHeight, deltaTime);
    }
  }


}