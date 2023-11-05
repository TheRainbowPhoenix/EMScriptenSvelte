<script lang="ts">
  import { onMount } from "svelte";
  import create_module from "./hello.js";
  let hello: any;

  export let message = "";
  let count = 0;

  onMount(async () => {
    hello = await create_module();
    count = hello._add(4, 5);
    hello._Debug_printf(1, 2, "Hello from Svelte (real C)");
    hello.Debug_printf(1, 2, "Hello from Svelte (JS wrapper)");
  });
</script>

<div>
  <h1>{count}</h1>
</div>
