export class DialogueBox {
  constructor(options = {}) {
    this._fullText = null;
    this._displayText = null;
    this.options = {
      fontSize: 24,
      fontFamily: 'omori-normal',
      lineHeight: 1.25,
      padding: 15,
      textColor: 'white',
      speed: 1, // characters per frame (default)
      ...options
    };
    
    this.isActive = false;
    this.currentIndex = 0;
    this.isAnimating = false;
    this.isComplete = false;
    this.animationFrameId = null;
    this.lastUpdateTime = 0;
  }

  set text(val) {
    if (!(typeof val === 'string')) {
      throw new Error("Dialogue box text must be a string you dummy");
    }

    this._fullText = val;
    this._displayText = "";
  }

  // Start or resume the text animation
  startAnimation(ctx, vw, vh, deltaTime, onComplete = () => {}) {
    if (this.animationFrameId) {
      console.log('cancelling');
      cancelAnimationFrame(this.animationFrameId);
    }

    console.log('started');

    this.isAnimating = true;
    this.isComplete = false;
    let accumulatedTime = 0;
    const frameDuration = 16.67;
    
    const animate = (timestamp) => {
        // Update text based on the passed deltaTime
        /*accumulatedTime += deltaTime;

        // Only update when we've accumulated enough time for a frame
        while (accumulatedTime >= frameDuration && !this.isComplete) {
            this.updateText();
            accumulatedTime -= frameDuration;
        }
        
        // Always draw every frame for smooth rendering
        this.draw(ctx, vw, vh);
        
        if (this.isComplete) {
            onComplete();
            return;
        }
        
        */
      this.draw(ctx, vw, vh);
      this.animationFrameId = requestAnimationFrame(animate);
    };
    
    this.animationFrameId = requestAnimationFrame(animate)
  }

  // Skip to complete text immediately
  completeAnimation() {
    this._displayText = this.fullText;
    this.currentIndex = this.fullText.length;
    this.isComplete = true;
    if (this.animationFrameId) {
      cancelAnimationFrame(this.animationFrameId);
      this.animationFrameId = null;
    }
  }

  // Update the displayed text portion
  updateText() {
    if (this.isComplete) return;
    
    const charsToAdd = Math.ceil(this.options.speed);
    this.currentIndex = Math.min(
      this.currentIndex + charsToAdd,
      this.fullText.length
    );
    
    this._displayText = this.fullText.substring(0, this.currentIndex);
    this.isComplete = this.currentIndex >= this.fullText.length;
  }

  draw(ctx, vw, vh) {
    if (!this.isActive) return;

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
    const lines = this.wrapText(ctx, this._displayText, effectiveWidth);
    this.drawTextLines(ctx, lines, textX, textY, effectiveHeight);
  }

    wrapText(ctx, text, maxWidth) {
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

  drawTextLines(ctx, lines, x, y, maxHeight) {
    const lineHeight = this.options.fontSize * this.options.lineHeight;
    const maxLines = Math.floor(maxHeight / lineHeight);

    if (lines.length > maxLines) {
      throw new Error(`Text overflow: ${lines.length} lines (max ${maxLines})`);
    }

    lines.forEach((line, i) => {
      ctx.fillText(line, x, y + (i * lineHeight));
    });
  }

  // Clean up animation frame when done
  destroy() {
    if (this.animationFrameId) {
      cancelAnimationFrame(this.animationFrameId);
    }
  }
}