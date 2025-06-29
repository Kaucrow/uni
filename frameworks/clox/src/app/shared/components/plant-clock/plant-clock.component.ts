import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-plant-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './plant-clock.component.html',
  styleUrls: ['./plant-clock.component.css']
})
export class PlantClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  
  plantState = {
    height: 0,
    leaves: [] as {x: number, y: number, size: number, rotation: number, id: number}[],
    fruits: [] as {x: number, y: number, size: number, id: number}[],
    branches: [] as {
  angle: number, 
  length: number, 
  position: number, 
  id: number, 
  side: 'left'|'right',
}[],
    currentSeconds: 0,
    currentMinutes: 0,
    currentHour: 0,
    lastHour: -1,
    lastMinute: -1
  };
  
  private intervalId: any;
  private nextLeafId = 0;
  private nextFruitId = 0;
  private nextBranchId = 0;

  ngOnInit(): void {
    this.updatePlant();
    this.intervalId = setInterval(() => {
      this.updatePlant();
    }, 1000);
  }

  ngOnDestroy(): void {
    if (this.intervalId) {
      clearInterval(this.intervalId);
    }
  }

  ngOnChanges(): void {
    this.updatePlant();
  }

  private updatePlant(): void {
    const currentSeconds = this.time.getSeconds();
    const currentMinutes = this.time.getMinutes();
    const currentHour = this.time.getHours();

    // Update current time display
    this.plantState.currentSeconds = currentSeconds;
    this.plantState.currentMinutes = currentMinutes;
    this.plantState.currentHour = currentHour;

    // Handle hour change (grow taller, add branches)
    if (currentHour !== this.plantState.lastHour) {
      this.plantState.height = currentHour;
      this.addBranches(currentHour);
      this.plantState.lastHour = currentHour;
    }

    // Handle minute change (update fruits)
    if (currentMinutes !== this.plantState.lastMinute) {
      this.updateFruits(currentMinutes);
      this.plantState.lastMinute = currentMinutes;
    }

    // Handle second change (update leaves)
    this.updateLeaves(currentSeconds);
  }

private addBranches(hours: number): void {
  const targetBranchCount = Math.max(42, Math.floor(hours / 2) + 1);
  
  while (this.plantState.branches.length < targetBranchCount) {
    const position = 0.45 + (this.plantState.branches.length / targetBranchCount) * 0.7;
    const isLeft = this.plantState.branches.length % 2 === 0;
    const angle = -45 + Math.random() * 20; // 20-40 degrees
    
    this.plantState.branches.push({
      angle: isLeft ? -angle : angle,
      length: 200 * (1 - position) + Math.random() * 30,
      position: position,
      id: this.nextBranchId++,
      side: isLeft ? 'left' : 'right',
    });
  }
}

  private updateLeaves(seconds: number): void {
    // Remove excess leaves if seconds decreased
    if (seconds < this.plantState.leaves.length) {
      this.plantState.leaves = this.plantState.leaves.slice(0, seconds);
    }
    
    // Add new leaves if needed
    while (this.plantState.leaves.length < seconds && this.plantState.branches.length > 0) {
      const branchIndex = Math.floor(Math.random() * this.plantState.branches.length);
      const branch = this.plantState.branches[branchIndex];
      const branchProgress = 0.3 + Math.random() * 0.7;
      
      const branchX = branchProgress * branch.length * Math.sin(branch.angle * Math.PI/180);
      const branchY = branchProgress * branch.length * Math.cos(branch.angle * Math.PI/180);
      
      this.plantState.leaves.push({
        x: 50 + branchX / 2.5,
        y: 80 - (branch.position * 90) - 25,
        size: 5 + Math.random() * 10,
        rotation: Math.random() * 360,
        id: this.nextLeafId++
      });
    }
  }

  private updateFruits(minutes: number): void {
    // Remove excess fruits if minutes decreased
    if (minutes < this.plantState.fruits.length) {
      this.plantState.fruits = this.plantState.fruits.slice(0, minutes);
    }
    
    // Add new fruits if needed
    while (this.plantState.fruits.length < minutes && this.plantState.branches.length > 0) {
      const branchIndex = Math.floor(Math.random() * this.plantState.branches.length);
      const branch = this.plantState.branches[branchIndex];
      const branchProgress = 0.5 + Math.random() * 0.4; // Place fruits more toward the ends
      
      const branchX = branchProgress * branch.length * Math.sin(branch.angle * Math.PI/180);
      const branchY = branchProgress * branch.length * Math.cos(branch.angle * Math.PI/180);

      this.plantState.fruits.push({
        x: 50 + branchX / 3.5,
        y: 90 - (branch.position * 100) - 25,
        size: 7 + Math.random() * 2,
        id: this.nextFruitId++
      });
    }
  }

  trackByFn(index: number, item: any): number {
    return item.id;
  }
}