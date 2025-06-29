import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-hex-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './hex-clock.component.html',
  styleUrls: ['./hex-clock.component.css']
})
export class HexClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  @Input() showSeconds: boolean = true;
  @Input() blinkSeparators: boolean = true;
  
  hexTime = {
    hours: '',
    minutes: '',
    seconds: ''
  };
  showColons: boolean = true;
  private intervalId: any;

  ngOnInit(): void {
    this.updateTime();
    this.intervalId = setInterval(() => {
      this.updateTime();
      if (this.blinkSeparators) {
        this.showColons = !this.showColons;
      }
    }, 500); // Blink every 500ms
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

    this.hexTime = {
      hours: this.toHex(hours),
      minutes: this.toHex(minutes),
      seconds: this.toHex(seconds)
    };
  }

  private toHex(num: number): string {
    return num.toString(16).toUpperCase().padStart(2, '0');
  }
}