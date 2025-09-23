<script lang="ts">
  import * as Card from "$lib/components/ui/card";
  import { Input } from "$lib/components/ui/input";
  import { Label } from "$lib/components/ui/label";
  import { Button } from "$lib/components/ui/button";
  import { Toggle } from "$lib/components/ui/toggle";

  import { goto } from "$app/navigation";

  import IconUser from "@tabler/icons-svelte/icons/user";
  import IconEye from "@tabler/icons-svelte/icons/eye";

  import { backend } from "$const";

  let isPasswordVisible: boolean = $state(false);

  let email: string = $state('');
  let password: string = $state('');

  let loginError: string | null = $state(null);

  async function logIn() {
    console.log(`Logging in with email ${email} & password ${password}...`);

    const res = await fetch(`${backend.url}/auth/login`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        email,
        password
      }),
      credentials: 'include'
    });

    // Handle errors
    if (!res.ok) {
      const errData = await res.json();
      console.error(errData.message);

      switch (res.status) {
        // 401 Unauthorized
        case 401: {
          loginError = 'No user was found with this email and password.';
          break;
        }
        // Other errors
        default: {
          loginError = 'Failed to log in.';
          break;
        }
      }

      return;
    }

    // Login successful
    console.log('Login successful! :)');
    goto('/user');
  }
</script>

<div class="flex flex-col h-[100vh] justify-center">
  <div class="flex flex-row justify-center">
    <Card.Root class="w-100">
      <!-- Header -->
      <Card.Header>
        <div class="flex flex-row gap-4 items-center">
          <IconUser />
          <div>
            <Card.Title>Welcome back!</Card.Title>
            <Card.Description>Log in to your account</Card.Description>
          </div>
        </div>
      </Card.Header>
      <Card.Content>
        <form onsubmit={logIn}>
          <!-- Inputs container -->
          <div class="flex flex-col gap-6">
            <!-- Email -->
            <div class="grid gap-2">
              <Label for="email">Email</Label>
              <Input
                bind:value={email}
                id="email"
                type="email"
                placeholder="user@example.com"
                required
              />
            </div>

            <!-- Password -->
            <div class="grid gap-2">
              <Label for="password">Password</Label>
              <div class="relative">
                <Input
                  bind:value={password}
                  id="password"
                  type={isPasswordVisible ? "text" : "password"}
                  class="pr-10"
                  required
                />
                <!-- Password visibility toggle -->
                <Toggle
                  bind:pressed={isPasswordVisible}
                  variant="ghost"
                  class="absolute top-0 right-1"
                >
                  <IconEye class="size-5" />
                </Toggle>
              </div>
            </div>

            <!-- Log In error -->
            {#if loginError}
              <div class="flex text-destructive text-sm justify-center">
                {loginError}
              </div>
            {/if}

            <!-- Log In button -->
            <div class="flex flex-row justify-center">
              <Button type="submit" class="w-1/3">Log In</Button>
            </div>
          </div>
        </form>
      </Card.Content>
    </Card.Root>
  </div>
</div>