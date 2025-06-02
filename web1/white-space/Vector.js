export class Vector2 {
  constructor(arg1, arg2) {
    // Case 1: Direction vector (x, y numbers)
    if (typeof arg1 === 'number' && typeof arg2 === 'number') {
      this.start = { x: 0, y: 0 };          // Default origin
      this.end = { x: arg1, y: arg2 };      // Direction vector
    }
    // Case 2: Edge vector ([x0,y0], [x1,y1] arrays)
    else if (Array.isArray(arg1) && Array.isArray(arg2)) {
      this.start = { x: arg1[0], y: arg1[1] };
      this.end = { x: arg2[0], y: arg2[1] };
    }
    // Invalid arguments
    else {
      throw new Error(`
        Invalid arguments. Use either:
        - new Vector2(x, y) for direction vectors, or
        - new Vector2([x0,y0], [x1,y1]) for edge vectors
      `);
    }
  }

  get directionVector() {
    return new Vector2(
      this.end.x - this.start.x,
      this.end.y - this.start.y
    );
  }

  get normalized() {
    const magnitude = this.magnitude;
    const dirV = this.directionVector;

    return new Vector2(
      dirV.end.x / magnitude,
      dirV.end.y / magnitude
    )
  }

  get magnitude() {
    const dirV = this.directionVector;

    return Math.sqrt(dirV.end.x ** 2 + dirV.end.y ** 2);
  }
}