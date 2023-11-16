<script lang="ts">
  import Hello from "./lib/Hello.svelte";
  export let ide = true;
</script>

<main>
  <Hello {ide}/>
</main>
