<script lang="ts">
  import { onMount } from "svelte";
  import { goto } from "$app/navigation";

  import * as Card from "$lib/components/ui/card";
  import { Button } from "$lib/components/ui/button";

  import IconMoodSmileBeam from "@tabler/icons-svelte/icons/mood-smile-beam";

  import { backend } from "$const";
  import type { UserData } from "$types/responses/user";

  let userName: string | null = $state(null);
  let userSurname: string | null = $state(null);

  onMount(async () => {
    console.log('Getting user data...');

    const res = await fetch(`${backend.url}/user-data`, {
      method: 'GET',
      credentials: 'include'
    });

    // Handle errors
    if (!res.ok) {
      const errData = await res.json();
      console.error(errData.message);

      switch (res.status) {
        default: {
          goto('/login');
          break;
        }
      }

      return;
    }

    const userData: UserData = await res.json();
    userName = userData.name;
    userSurname = userData.surname;
  });

  async function logOut() {
    console.log('Logging out...');

    const res = await fetch(`${backend.url}/auth/logout`, {
      method: 'POST',
      credentials: 'include'
    });

    // Handle errors
    if (!res.ok) {
      const errData = await res.json();
      console.error(errData.message);
      return;
    }

    // If logout successful, go back to login
    goto('/login');
  }
</script>

{#if userName}
  <div class="flex flex-col h-[100vh] justify-center">
    <div class="flex flex-row justify-center">
      <Card.Root class="w-100">
        <!-- Header -->
        <Card.Header>
          <div class="flex flex-row gap-4 items-center">
            <IconMoodSmileBeam />
            <div class="flex flex-col gap-2">
              <Card.Title>You're logged in!</Card.Title>
              <Card.Description>It's nice to see you again, {userName}</Card.Description>
            </div>
          </div>
        </Card.Header>
        <Card.Content>
          <div class="flex flex-row justify-center">
            <Button onclick={logOut} class="w-1/3">Log Out</Button>
          </div>
        </Card.Content>
      </Card.Root>
    </div>
  </div>
{/if}