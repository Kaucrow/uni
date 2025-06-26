import { Routes } from '@angular/router';
import { HomeComponent } from './pages/home/home.component';
import { RegisterComponent } from './pages/register/register.component';
import { LoginComponent } from './pages/login/login.component';
import { ClocksComponent } from './pages/clocks/clocks.component'; 

export const routes: Routes = [
  {
    path: 'register',
    component: RegisterComponent,
    title: 'Register'
  },
  {
    path: 'login',
    component: LoginComponent
  },
  {
    path: 'clocks',
    component: ClocksComponent
  },
  {
    path: '',
    component: HomeComponent,
    title: 'Home'
  }
];