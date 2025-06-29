import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-binary-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './binary-clock.component.html',
  styleUrls: ['./binary-clock.component.css']
})
export class BinaryClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  @Input() showSeconds: boolean = true;
  
  binaryTime = {
    hours: '',
    minutes: '',
    seconds: ''
  };
  private intervalId: any;

  ngOnInit(): void {
    this.updateTime();
    if (this.showSeconds) {
      this.intervalId = setInterval(() => {
        this.updateTime();
      }, 1000);
    }
  }

  ngOnDestroy(): void {
    if (this.intervalId) {
      clearInterval(this.intervalId);
    }
  }

  ngOnChanges(): void {
    this.updateTime();
  }

  private updateTime(): void {
    const hours = this.time.getHours();
    const minutes = this.time.getMinutes();
    const seconds = this.time.getSeconds();

    this.binaryTime = {
      hours: this.toBinary(hours, 5),    // Hours need 5 bits (0-23)
      minutes: this.toBinary(minutes, 6), // Minutes need 6 bits (0-59)
      seconds: this.toBinary(seconds, 6)  // Seconds need 6 bits (0-59)
    };
  }

  private toBinary(num: number, bits: number): string {
    return num.toString(2).padStart(bits, '0');
  }
}