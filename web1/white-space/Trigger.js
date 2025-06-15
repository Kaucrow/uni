import { Collider } from "./Collider.js";

export class Trigger extends Collider {
  constructor(config, parent) {
    super(config, parent);
    this.isTrigger = true;
    this.onEnter = config.onEnter || (() => {});
    this.onExit = config.onExit || (() => {});
    this.onStay = config.onStay || (() => {});
    this.interactsWith = config.interactsWith;
    this.requiredFacing = config.requiredFacing || null; // 'up', 'down', 'left', 'right'
    this._objects = [];
  }

  checkFacing(player) {
    if (!this.requiredFacing) return true;
    return player.facingDirection === this.requiredFacing;
  }

  update(source, isNowInTrigger) {
    const isSourceInTrigger = this._objects.includes(source);
    if (isNowInTrigger && !isSourceInTrigger) {
      this._objects.push(source);

      this.onEnter(source, this.parent);
    } else if (!isNowInTrigger && isSourceInTrigger) {
      const objIdx = this._objects.findIndex(obj => obj === source);
      if (objIdx !== -1) {
        this._objects.splice(objIdx, 1);
      } else {
        throw new Error("Attempted to remove a source that is not in the objects list");
      }

      this.onExit(source, this.parent);
    } else if (isNowInTrigger) {
      this.onStay(source, this.parent);
    }
  }
}