<template>
  <main class="container">
    <div v-if="isLoggedIn">Email: {{ identity.email }}</div>
    <div v-if="isReady" class="locking-mechanism">
      <CircleButton
          class="button"
          :label="isLocked ? 'Locked' : 'Unlocked'"
          :color="isLocked ? 'green' : 'red'"
          @click="isLocked = !isLocked"
      />
    </div>
    <div v-else class="loading">
      <div v-for="_ in 3" class="dot"></div>
    </div>
  </main>
</template>

<script setup lang="ts">
import {computed, onMounted, ref} from "vue";
import {openUrl} from "@tauri-apps/plugin-opener";
import {buildLoginUrl} from "./util/auth.ts";
import {getCurrent} from "@tauri-apps/plugin-deep-link";

import CircleButton from "./component/CircleButton.vue";

const token = ref<string | null>(null);
const identity = computed(() => {
  if (!token.value) return null;

  const [_, payload] = token.value.split(".");
  const identity = JSON.parse(atob(payload));
  console.log(identity);
  if (identity.exp > Date.now() / 1000) {
    return identity;
  }

  return null;
});

const isLoggedIn = computed(() => !!identity.value);
const isConnected = ref<boolean>(false);

const isReady = computed(() => {
  return isLoggedIn.value && isConnected.value;
});

const isLocked = ref<boolean>(false);

onMounted(async () => {
  // Check if callback from deeplink
  const startUrls = await getCurrent();
  if (startUrls) {
    for (const raw of startUrls) {
      const url = new URL(raw);

      const code = url.searchParams.get("code");
      if (code) {
        const response = await fetch(`${import.meta.env.VITE_AUTH_AUTHORITY}/oauth2/token`, {
          method: "POST",
          body: new URLSearchParams({
            "grant_type": "authorization_code",
            "code": code,
            "client_id": import.meta.env.VITE_AUTH_CLIENT_ID,
            "redirect_uri": "doorctl://auth",
          }),
        });

        if (response.ok) {
          const payload = await response.json();

          token.value = payload.id_token;
        }
      }
    }
  }
  // Check if user credentials are stored
  // IF YES
  // - Start pinging for access tokens
  // IF NO
  // - Redirect to login page
  if (!isLoggedIn.value) {
    const loginUrl = buildLoginUrl({
      authority: import.meta.env.VITE_AUTH_AUTHORITY,
      clientId: import.meta.env.VITE_AUTH_CLIENT_ID,
      redirectUri: 'doorctl://auth',
      responseType: 'code',
      scope: ['email', 'openid'],
    });
    await openUrl(loginUrl);
  }
});
</script>

<style scoped lang="scss">
.container {
  height: 100%;
  width: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;

  .button {
    width: 10em;
    height: 10em;
  }
}
</style>