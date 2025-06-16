import { Dialogue } from "./Dialogue.js";

export class DialogueBox {
  constructor(room, options = {}) {
    this.options = {
      fontSize: 26,
      fontFamily: 'omori-normal',
      lineHeight: 1.15,
      padding: 15,
      textColor: 'white',
      ...options
    };

    this.room = room;

    this._dialogue = null;
    this._fullText = null;
    this._displayText = null;

    this._isActive = false;
    this._dialogueIndex = 0;
    this._textIndex = 0;
    this._isAnimating = false;
    this._animationTimer = 0;
    this._speed = null;
  }

  set text(val) {
    if (!(typeof val === 'string')) {
      throw new Error("Dialogue box text must be a string you dummy");
    }

    this._fullText = val;
    this._displayText = "";
  }

  get isActive() {
    return this._isActive;
  }

  get completed() {
    return !this._isAnimating;
  }

  enable(obj) {
    const dialogue = this.#getDialogue(obj);

    if (!Array.isArray(dialogue) || !dialogue.every(item => item instanceof Dialogue)) throw new Error("dialogue must be an array of instances of Dialogue");

    this._dialogue = dialogue;
    this._isActive = true;
    this._isAnimating = true;
    this._fullText = dialogue[0].text;
    this._displayText = "";
    this._speed = dialogue[0].speed;
  }

  advanceDialogue() {
    if (this._dialogueIndex === this._dialogue.length - 1) {
      this._dialogue = null;
      this._dialogueIndex = 0;
      this._isActive = false;
      this._fullText = null;
      this._textIndex = null;
      this._animationTimer = 0;
      this._displayText = null;
      this._speed = null;
    } else {
      this._dialogueIndex++;
      this._textIndex = 0;
      this._animationTimer = 0;
      this._fullText = this._dialogue[this._dialogueIndex].text;
      this._displayText = "";
      this._speed = this._dialogue[this._dialogueIndex].speed;
      this._isAnimating = true;
    }
  }

  speedUpDialogue() {
    this._textIndex = this._fullText.length;
  }

  update(deltaTime) {
    this._animationTimer += deltaTime * this._speed;

    if (this._animationTimer >= 10) {
      this._animationTimer -= 10;
      this.#updateText();
    }
  }

  draw(ctx, vw, vh) {
    if (!this._isActive) return;

    if (!ctx) throw new Error("No canvas context provided");

    const align = (x) => Math.round(x) + 0.5;
    const hMargin = align(vw > 700 ? vw / 4 : 20);
    const vMargin = 20;
    const width = align(vw > 700 ? 660 : vw - hMargin * 2);
    const height = 120;
    const boxY = align(vh - height - vMargin);

    // Draw box
    const thinBorder = 1;
    const thickBorder = 3;
    
    ctx.fillStyle = 'black';
    ctx.fillRect(hMargin, boxY, width, height);

    ctx.strokeStyle = 'white';
    ctx.lineWidth = thickBorder;
    ctx.strokeRect(
      hMargin + thinBorder * 2 + 1,
      boxY + thinBorder * 2 + 1,
      width - thinBorder - thickBorder - 2,
      height - thinBorder - thickBorder - 2
    );

    // Prepare text rendering
    const effectiveWidth = width - this.options.padding * 2;
    const effectiveHeight = height - this.options.padding * 2;
    const textX = hMargin + this.options.padding;
    const textY = boxY + this.options.padding;

    ctx.font = `${this.options.fontSize}px ${this.options.fontFamily}`;
    ctx.fillStyle = this.options.textColor;
    ctx.textBaseline = 'top';

    // Wrap and draw the current display text
    const lines = this.#wrapText(ctx, this._displayText, effectiveWidth);
    this.#drawTextLines(ctx, lines, textX, textY, effectiveHeight);
  }

  #getDialogue(obj) {
    const dialogue = this.room.dialogues[obj.id];

    if (!dialogue) throw new Error(`No dialogue was found for object with id: '${obj.id}'`)

    return dialogue;
  }

  #wrapText(ctx, text, maxWidth) {
    const words = text.split(' ');
    const lines = [];
    let currentLine = words[0];

    for (let i = 1; i < words.length; i++) {
      const word = words[i];
      const testLine = currentLine + ' ' + word;
      const metrics = ctx.measureText(testLine);

      if (metrics.width <= maxWidth) {
        currentLine = testLine;
      } else {
        lines.push(currentLine);
        currentLine = word;
      }
    }
    lines.push(currentLine);
    return lines;
  }

  #drawTextLines(ctx, lines, x, y, maxHeight) {
    const lineHeight = this.options.fontSize * this.options.lineHeight;
    const maxLines = Math.floor(maxHeight / lineHeight);

    if (lines.length > maxLines) {
      throw new Error(`Text overflow: ${lines.length} lines (max ${maxLines})`);
    }

    lines.forEach((line, i) => {
      ctx.fillText(line, x, y + (i * lineHeight));
    });
  }

  // Update the displayed text portion
  #updateText() {
    if (!this._isAnimating) return;

    const charsToAdd = 1;
    this._textIndex = Math.min(
      this._textIndex + charsToAdd,
      this._fullText.length
    );

    while (this._fullText[this._textIndex] === ' ') {
      this._textIndex = Math.min(
        this._textIndex + charsToAdd,
        this._fullText.length
      );
    }
    
    this._displayText = this._fullText.substring(0, this._textIndex);
    this._isAnimating = this._textIndex < this._fullText.length;
  }
}