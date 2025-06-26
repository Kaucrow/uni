import { Injectable, signal } from "@angular/core";
import { Observable, from } from "rxjs";
import { AuthResponse, createClient } from '@supabase/supabase-js';
import { environment } from "../../environments/environment.development";

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  supabase = createClient(environment.supabaseUrl, environment.supabaseKey);
  currentUser = signal<{ email: string; username: string } | null>(null);

  register(
    email: string,
    username: string,
    password: string
  ): Observable<AuthResponse> {
    const promise = this.supabase.auth.signUp({
      email,
      password,
      options: {
        data: {
          username,
        },
      },
    });
    return from(promise);
  }

  login(email: string, password: string): Observable<AuthResponse> {
    const promise = this.supabase.auth.signInWithPassword({
      email,
      password
    });
    console.log(`!! Logging in with email '${email}' and password '${password}'...`);
    return from(promise);
  }

  logout(): Observable<{ error: Error | null }> {
    const promise = this.supabase.auth.signOut();
    return from(promise);
  }
}