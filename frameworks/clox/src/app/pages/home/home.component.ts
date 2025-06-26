import { Component, OnInit, inject } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {
  isLoggedIn: boolean = false;
  username: string = '';
  email: string = '';
  authService = inject(AuthService);
  router = inject(Router);

  ngOnInit(): void {
    setTimeout(() => {
      if (this.authService.currentUser()) {
        this.isLoggedIn = true;
        const user = this.authService.currentUser();
        this.username = user?.username || '';
        this.email = user?.email || '';
      }
    }, 0);
  }

  navigateToClocks(): void {
    this.router.navigate(['/clocks']);
  }

  navigateToSignIn(): void {
    this.router.navigate(['/login']);
  }

  navigateToRegister(): void {
    this.router.navigate(['/register']);
  }

  handleSignOut(): void {
    this.authService.logout();
    this.isLoggedIn = false;
  }
}``