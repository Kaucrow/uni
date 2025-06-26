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
  selector: 'app-login',
  standalone: true,
  imports: [
    CommonModule,
    InputComponent,
    CheckboxComponent,
    CenteredDivComponent,
    ReactiveFormsModule,
    RouterModule,
  ],
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css'],
})
export class LoginComponent implements OnInit {
  fb = inject(FormBuilder);
  http = inject(HttpClient);
  router = inject(Router);
  authService = inject(AuthService);
  isSigningIn: boolean = false;
  rememberMe: boolean = false;
  err: { email: string; notFound: string } = { email: '', notFound: '' };
  errorMessage!: string;

  form = this.fb.nonNullable.group({
    email: ['', Validators.required],
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
    if (this.isSigningIn) return;

    try {
      this.isSigningIn = true;

      const rawForm = this.form.getRawValue()
      this.authService.login(
        rawForm.email, rawForm.password
      ).subscribe(result => {
        if (result.error) {
          this.errorMessage = result.error.message;
        } else {
          this.router.navigateByUrl('/');
        }
      })
    } catch (error) {
      console.error(error);
      this.isSigningIn = false;
    }
  }
}