export class RandomSpawner {
  constructor(width, height, minSpacing = 500, borderMargin = 700, maxSpawnAttempts = 100) {
    this.width = width;
    this.height = height;
    this.minSpacing = minSpacing;
    this.borderMargin = borderMargin;
    this.maxSpawnAttempts = maxSpawnAttempts;
    this.spawnedObjects = [];
  }

  spawnObjects(count, factoryFn) {
    const newObjects = [];
    
    for (let i = 0; i < count; i++) {
      const position = this.findValidPosition();
      if (!position) break; // No valid position found
      
      const obj = {
        id: newObjects.length,
        instance: factoryFn(position.x, position.y),
      };

      this.spawnedObjects.push(obj);
      newObjects.push(obj);
    }
    
    return newObjects;
  }

  findValidPosition() {
    for (let attempt = 0; attempt < this.maxSpawnAttempts; attempt++) {
      const position = this.generateRandomPosition();
      
      if (this.isPositionValid(position.x, position.y)) {
        return position;
      }
    }
    return null;
  }

  generateRandomPosition() {
    if (Math.random() < 0.5) {
      // X-axis aligned (left/right halves)
      const randomX = Math.random() < 0.5
        ? this.borderMargin + Math.random() * (this.width/2 - this.borderMargin*2)
        : this.width/2 + this.borderMargin + Math.random() * (this.width/2 - this.borderMargin*2);
      
      const randomY = this.borderMargin + Math.random() * (this.height - this.borderMargin*2);
      return { x: Math.round(randomX), y: Math.round(randomY) };
    } else {
      // Y-axis aligned (top/bottom halves)
      const randomX = this.borderMargin + Math.random() * (this.width - this.borderMargin*2);
      
      const randomY = Math.random() < 0.5
        ? this.borderMargin + Math.random() * (this.height/2 - this.borderMargin*2)
        : this.height/2 + this.borderMargin + Math.random() * (this.height/2 - this.borderMargin*2);
      
      return { x: Math.round(randomX), y: Math.round(randomY) };
    }
  }

  isPositionValid(x, y) {
    return !this.spawnedObjects.some(obj => {
      const dx = obj.instance.x - x;
      const dy = obj.instance.y - y;
      return Math.sqrt(dx*dx + dy*dy) < this.minSpacing;
    });
  }

  clearObjects() {
    this.spawnedObjects = [];
  }

  get objects() {
    return this.spawnedObjects;
  }
}