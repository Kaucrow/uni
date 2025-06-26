import { Component, Input, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common'; // Required for ngClass and ngStyle

@Component({
  selector: 'app-centered-div',
  standalone: true,
  imports: [CommonModule], // Import CommonModule for ngClass and ngStyle
  templateUrl: './centered-div.component.html',
  styleUrls: ['./centered-div.component.css'],
})
export class CenteredDivComponent implements OnInit {
  @Input() className: string = '';
  @Input() backgroundColor: string = 'transparent';
  @Input() height: string = '100%';
  @Input() padding: string = '0px';
  @Input() paddingTop: string = '0px';

  constructor() {}

  ngOnInit(): void {}

  // A getter to easily construct the styles object for [ngStyle]
  get outerContainerStyles() {
    return {
      'background-color': this.backgroundColor,
      height: this.height,
      padding: this.padding,
      'padding-top': this.paddingTop,
      // If you intended to use CSS variables, you'd define them in the CSS
      // or set them directly like this. Angular doesn't directly support
      // passing CSS variables to style via property binding in the same way React does.
      // If you really need CSS variables set inline, you might need a HostBinding
      // or Renderer2, but direct style binding is usually preferred.
    };
  }
}