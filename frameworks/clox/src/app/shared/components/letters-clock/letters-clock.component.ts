import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-letters-clock',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './letters-clock.component.html',
  styleUrls: ['./letters-clock.component.css']
})
export class LettersClockComponent implements OnInit, OnDestroy {
  @Input() time: Date = new Date();
  timeInWords: string = '';
  private intervalId: any;

  private numbers = [
    'cero', 'uno', 'dos', 'tres', 'cuatro', 'cinco', 
    'seis', 'siete', 'ocho', 'nueve', 'diez', 'once',
    'doce', 'trece', 'catorce', 'quince', 'dieciséis',
    'diecisiete', 'dieciocho', 'diecinueve', 'veinte',
    'veintiuno', 'veintidós', 'veintitrés', 'veinticuatro',
    'veinticinco', 'veintiséis', 'veintisiete', 'veintiocho',
    'veintinueve', 'treinta', 'treinta y uno', 'treinta y dos',
    'treinta y tres', 'treinta y cuatro', 'treinta y cinco',
    'treinta y seis', 'treinta y siete', 'treinta y ocho',
    'treinta y nueve', 'cuarenta', 'cuarenta y uno', 'cuarenta y dos',
    'cuarenta y tres', 'cuarenta y cuatro', 'cuarenta y cinco',
    'cuarenta y seis', 'cuarenta y siete', 'cuarenta y ocho',
    'cuarenta y nueve', 'cincuenta', 'cincuenta y uno', 'cincuenta y dos',
    'cincuenta y tres', 'cincuenta y cuatro', 'cincuenta y cinco',
    'cincuenta y seis', 'cincuenta y siete', 'cincuenta y ocho',
    'cincuenta y nueve'
  ];

  ngOnInit(): void {
    this.updateTime();
    this.intervalId = setInterval(() => {
      this.updateTime();
    }, 1000);
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
    const period = hours < 12 ? 'de la mañana' : 'de la tarde';

    const displayHours = hours % 12 || 12;
    
    this.timeInWords = 
      `${this.getHourText(displayHours)}, ` +
      `${this.numbers[minutes]} minuto${minutes !== 1 ? 's' : ''}, ` +
      `${this.numbers[seconds]} segundo${seconds !== 1 ? 's' : ''}, ` +
      `${period}`;
  }

  private getHourText(hours: number): string {
    // Special case for 1 (feminine "una" instead of "uno")
    if (hours === 1) return 'una hora';
    return `${this.numbers[hours]} hora${hours !== 1 ? 's' : ''}`;
  }
}