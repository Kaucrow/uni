import { Component, Input, Output, EventEmitter, OnInit, forwardRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule, ControlValueAccessor, NG_VALUE_ACCESSOR } from '@angular/forms';

@Component({
  selector: 'app-input',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './input.component.html',
  styleUrls: ['./input.component.css'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      useExisting: forwardRef(() => InputComponent),
      multi: true
    }
  ]
})
export class InputComponent implements OnInit, ControlValueAccessor {
  @Input() label: string = '';
  @Input() id: string = 'input';
  @Input() required: boolean = false;
  @Input() type: string = 'text';
  @Input() err: boolean = false;

  public _inputValue: string = '';

  onChange = (val: any) => {};
  onTouched = () => {};

  writeValue(val: any): void {
    this._inputValue = val;
  }

  registerOnChange(fn: any): void {
    this.onChange = fn;
  }

  registerOnTouched(fn: any): void {
    this.onTouched = fn;
  }

  constructor() {}

  ngOnInit(): void {}

  updateValue(event: Event): void {
    const target = event.target as HTMLInputElement;
    this._inputValue = target.value;
    this.onChange(this._inputValue);
    this.onTouched();
  }
}