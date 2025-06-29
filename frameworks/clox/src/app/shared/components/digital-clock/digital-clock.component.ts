import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-digital-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './digital-clock.component.html',
  styleUrls: ['./digital-clock.component.css']
})
export class DigitalClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  @Input() displaySeconds: boolean = true;
  @Input() militaryTime: boolean = false;
  @Input() blinkSeparators: boolean = true;
  
  formattedTime: string = '';
  showSeparators: boolean = true;
  private intervalId: any;

  ngOnInit(): void {
    this.updateTime();
    if (this.displaySeconds) {
      this.intervalId = setInterval(() => {
        this.updateTime();
        if (this.blinkSeparators) {
          this.showSeparators = !this.showSeparators;
        }
      }, 500);
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

    const displayHours = this.militaryTime ? hours : hours % 12 || 12;
    const ampm = this.militaryTime ? '' : hours < 12 ? 'AM' : 'PM';

    this.formattedTime = 
      `${this.padZero(displayHours)}${this.showSeparators ? ':' : ' '}${this.padZero(minutes)}` +
      (this.displaySeconds ? `${this.showSeparators ? ':' : ' '}${this.padZero(seconds)}` : '') +
      (ampm ? ` ${ampm}` : '');
  }

  private padZero(num: number): string {
    return num < 10 ? `0${num}` : `${num}`;
  }
}