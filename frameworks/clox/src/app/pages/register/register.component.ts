import { Component, OnInit, inject } from '@angular/core';
import { Router, RouterModule } from '@angular/router';
import { AuthService } from '../../services/auth.service';
import { CommonModule } from '@angular/common';
import { CheckboxComponent } from '../../shared/components/checkbox/checkbox.component';
import { InputComponent } from '../../shared/components/input/input.component';
import { CenteredDivComponent } from '../../shared/components/centered-div/centered-div.component';
import { FormBuilder, ReactiveFormsModule, Validators } from '@angular/forms';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-register',
  standalone: true,
  imports: [
    CommonModule,
    InputComponent,
    CheckboxComponent,
    CenteredDivComponent,
    ReactiveFormsModule,
    RouterModule,
  ],
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css'],
})
export class RegisterComponent implements OnInit {
  fb = inject(FormBuilder);
  http = inject(HttpClient);
  router = inject(Router);
  authService = inject(AuthService);
  isRegistering: boolean = false;
  err: { email: string; notFound: string } = { email: '', notFound: '' };
  errorMessage!: string;

  form = this.fb.nonNullable.group({
    email: ['', Validators.required],
    username: ['', Validators.required],
    password: ['', Validators.required],
  });

  ngOnInit(): void {
    setTimeout(() => {
      if (this.authService.currentUser()) {
        this.router.navigateByUrl('/');
      }
    }, 0);
  }

  async onSubmit(): Promise<void> {
    console.log('elatla');
    if (this.isRegistering) return;

    try {
      this.isRegistering = true;

      const rawForm = this.form.getRawValue()
      this.authService.register(
        rawForm.email, rawForm.username, rawForm.password
      ).subscribe(result => {
        if (result.error) {
          this.errorMessage = result.error.message;
          this.isRegistering = false;
        } else {
          this.router.navigateByUrl('/');
        }
      })
    } catch (error) {
      console.error(error);
      this.isRegistering = false;
    }
  }
}