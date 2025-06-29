import { Component, Input, OnDestroy, OnInit, inject } from '@angular/core';
import { Router } from '@angular/router';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { MatCardModule } from '@angular/material/card';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatButtonModule } from '@angular/material/button';
import { MatInputModule } from '@angular/material/input';
import { AuthService } from '../../services/auth.service';
import { AnalogClockComponent } from '../../shared/components/analog-clock/analog-clock.component';
import { DigitalClockComponent } from '../../shared/components/digital-clock/digital-clock.component';
import { LettersClockComponent } from '../../shared/components/letters-clock/letters-clock.component';
import { BinaryClockComponent } from '../../shared/components/binary-clock/binary-clock.component';
import { HexClockComponent } from '../../shared/components/hex-clock/hex-clock.component';
import { PlantClockComponent } from '../../shared/components/plant-clock/plant-clock.component';

@Component({
  selector: 'app-clocks',
  standalone: true,
  imports: [
    CommonModule,
    FormsModule,
    MatCardModule,
    MatGridListModule,
    MatButtonModule,
    MatInputModule,
    AnalogClockComponent,
    DigitalClockComponent,
    LettersClockComponent,
    BinaryClockComponent,
    HexClockComponent,
    PlantClockComponent
  ],
  templateUrl: './clocks.component.html',
  styleUrls: ['./clocks.component.css']
})
export class ClocksComponent implements OnInit, OnDestroy {
  @Input() clockCount: number = 6;
  currentTime: Date = new Date();
  hours: number = this.currentTime.getHours();
  minutes: number = this.currentTime.getMinutes();
  seconds: number = this.currentTime.getSeconds();
  fixedHours: number = this.currentTime.getHours();
  fixedMinutes: number = this.currentTime.getMinutes();
  fixedSeconds: number = this.currentTime.getSeconds();
  isShowingCustomTime: boolean = false;
  authService = inject(AuthService);
  router = inject(Router);
  private intervalId: any;

  clockTypes: string[] = ['analog', 'digital', 'letters', 'binary', 'hex', 'plant'];
  clocks: { type: string }[] = [];

  ngOnInit(): void {
    setTimeout(() => {
      if (!this.authService.currentUser()) {
        this.router.navigateByUrl('/');
      }
    }, 0);

    this.initializeClocks();
    this.startClock();
  }

  ngOnDestroy(): void {
    if (this.intervalId) {
      clearInterval(this.intervalId);
    }
  }

  initializeClocks(): void {
    this.clocks = Array(this.clockCount).fill(0).map((_, i) => ({
      type: this.clockTypes[i % this.clockTypes.length]
    }));
  }

  startClock(): void {
    this.intervalId = setInterval(() => {
      // Only update if we're not currently showing a custom time
      if (!this.isShowingCustomTime) {
        this.currentTime = new Date();
        this.hours = this.currentTime.getHours();
        this.minutes = this.currentTime.getMinutes();
        this.seconds = this.currentTime.getSeconds();
      }
    }, 1000);
  }

  getCurrentTime(): void {
    const now = new Date();
    this.hours = now.getHours();
    this.minutes = now.getMinutes();
    this.seconds = now.getSeconds();
    this.currentTime = new Date(now); // Update display time
    this.isShowingCustomTime = false;
  }

  setCustomTime(): void {
    // Validate inputs
    this.hours = Math.max(0, Math.min(23, this.fixedHours || 0));
    this.minutes = Math.max(0, Math.min(59, this.fixedMinutes || 0));
    this.seconds = Math.max(0, Math.min(59, this.fixedSeconds || 0));
    
    // Create new time from inputs
    this.currentTime = new Date();
    this.currentTime.setHours(this.hours, this.minutes, this.seconds);

    this.isShowingCustomTime = true;
  }

  getFormattedTime(): string {
    return this.currentTime.toLocaleTimeString();
  }
}