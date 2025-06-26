import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-analog-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './analog-clock.component.html',
  styleUrls: ['./analog-clock.component.css']
})
export class AnalogClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  @Input() size: number = 200; // Default size in pixels
  
  hourRotation: number = 0;
  minuteRotation: number = 0;
  secondRotation: number = 0;
  private intervalId: any;

  ngOnInit(): void {
    this.updateClockHands();
    // Update the second hand every second if showing live time
    this.intervalId = setInterval(() => {
      this.updateSecondHand();
    }, 1000);
  }

  ngOnDestroy(): void {
    if (this.intervalId) {
      clearInterval(this.intervalId);
    }
  }

  ngOnChanges(): void {
    this.updateClockHands();
  }

  private updateClockHands(): void {
    const hours = this.time.getHours();
    const minutes = this.time.getMinutes();
    const seconds = this.time.getSeconds();
    
    // Calculate rotations
    this.hourRotation = (hours % 12) * 30 + minutes * 0.5; // 30 degrees per hour + 0.5 per minute
    this.minuteRotation = minutes * 6; // 6 degrees per minute
    this.secondRotation = seconds * 6; // 6 degrees per second
  }

  private updateSecondHand(): void {
    // Only update the second hand (for smooth animation)
    this.secondRotation = this.time.getSeconds() * 6;
  }
}