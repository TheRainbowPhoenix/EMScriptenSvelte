<script lang="ts">
  import { onDestroy, onMount } from "svelte";
  import create_module from "./hello.js";
  let hello: any;

  export let message = "";
  let count = 0;

  let canvas: HTMLCanvasElement;
  let ctx: CanvasRenderingContext2D;

  onMount(async () => {
    ctx = canvas.getContext("2d");

    // @ts-ignore
    // document.Debug_printf = () =>
    //   // x: number,
    //   // y: number,
    //   // invert: boolean,
    //   // zero: number,
    //   // format: string
    //   {
    //     console.log("## Debug_printf ##");
    //     console.log(arguments);
    //   };

    document.addEventListener("calcInit", onCalcInit);
    document.addEventListener("calcEnd", onCalcEnd);
    document.addEventListener("onCall", onCall);

    hello = await create_module();
    console.log(hello);
    count = hello._add(4, 5);
    // hello.Debug_printf(10, 2, false, 0, "Hello from Svelte (JS wrapper)");
  });

  onDestroy(() => {
    document.removeEventListener("calcInit", onCalcInit);
    document.removeEventListener("calcEnd", onCalcEnd);
    document.removeEventListener("onCall", onCall);
  });

  interface OnCall {
    function: string;
    args: { [name: string]: any };
  }

  const onCall = (ev: Event) => {
    const details: OnCall = (ev as CustomEvent).detail;
    console.log("onCall:", details.function, " - ", details.args);

    if (details.function === "Debug_Printf") {
      message = details.args.format;

      ctx.font = "16px monospace";
      ctx.fillStyle = "white";
      ctx.fillText(message, details.args.x, details.args.y * 16);
    }

    ev.stopImmediatePropagation();
  };

  const onCalcInit = (ev: Event) => {
    console.log("calcInit:", (ev as CustomEvent).detail);
    ev.stopImmediatePropagation();
  };

  const onCalcEnd = (ev: Event | any) => {
    console.log("onCalcEnd:", (ev as CustomEvent).detail);
    ev.stopImmediatePropagation();
  };

  let code = `int main() {
  calcInit(); //backup screen and init some variables

  Debug_Printf(10,1,false,0,"HelloWorld%d",42);

  calcEnd(); //restore screen and do stuff

  return 0;
}`;
</script>

<div class="container">
  <div class="left">
    <textarea
      name="code"
      id="code"
      cols="30"
      rows="40"
      bind:value={code}
      readonly
    />
  </div>
  <div class="right">
    <span>hello._add(4,5): {count}</span>
    <p>
      Debug_Printf:
      <code>{message}</code>
    </p>

    <canvas width="320" height="528" bind:this={canvas} />
  </div>
</div>

<style>
  .container {
    width: 100%;
    display: flex;
    flex-direction: row;
  }
  .left {
    flex-grow: 1;
    width: 100%;
  }
  textarea {
    width: 100%;
  }

  .right {
    margin-left: 2rem;
    flex-basis: 450px;
  }
  canvas {
    width: 320px;
    height: 528px;
    background-color: black;
  }
</style>
