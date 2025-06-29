import { Collider } from "./Collider.js";
import { Vector2 } from "./Vector.js";

export class Trigger extends Collider {
  static newDownDialogue = (draw = false) => {
    return {
      draw: draw ? {} : null,
      group: 'dialogue',
      interactsWith: 'player',
      edges: [
        new Vector2([4, 22], [30, 22]),
        new Vector2([30, 22], [30, 32]),
        new Vector2([30, 32], [4, 32]),
        new Vector2([4, 32], [4, 22]),
      ],
    };
  };

  static newRightDialogue = (draw = false) => {
    return {
      draw: draw ? {} : null,
      group: 'dialogue',
      interactsWith: 'player',
      edges: [
        new Vector2([32, 0], [44, 0]),
        new Vector2([44, 0], [44, 30]),
        new Vector2([44, 30], [32, 30]),
        new Vector2([32, 30], [32, 0]),
      ]
    };
  };

  static newLeftDialogue = (draw = false) => {
    return {
      draw: draw ? {} : null,
      group: 'dialogue',
      interactsWith: 'player',
      edges: [
        new Vector2([0, 0], [-12, 0]),
        new Vector2([-12, 0], [-12, 30]),
        new Vector2([-12, 30], [0, 30]),
        new Vector2([0, 30], [0, 0]),
      ],
    };
  };

  static newUpDialogue = (draw = false) => {
    return {
      draw: draw ? {} : null,
      group: 'dialogue',
      interactsWith: 'player',
      edges: [
        new Vector2([4, -6], [30, -6]),
        new Vector2([30, -6], [30, 0]),
        new Vector2([30, 0], [4, 0]),
        new Vector2([4, 0], [4, -6]),
      ]
    };
  };

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

      source.inTriggers.push(this);
    } else if (!isNowInTrigger && isSourceInTrigger) {
      // Remove the source from the tracked objects
      const objIdx = this._objects.findIndex(obj => obj === source);
      if (objIdx !== -1) {
        this._objects.splice(objIdx, 1);
      } else {
        throw new Error("Attempted to remove a source that is not in the objects list");
      }

      // Remove the trigger from the tracked triggers in source
      const triggerIdx = source.inTriggers.findIndex(trigger => trigger === this);
      if (triggerIdx !== -1) {
        source.inTriggers.splice(triggerIdx, 1);
      } else {
        throw new Error("Attempted to remove a trigger that is not in the triggers list");
      }

      this.onExit(source, this.parent);
    } else if (isNowInTrigger) {
      this.onStay(source, this.parent);
    }
  }
}