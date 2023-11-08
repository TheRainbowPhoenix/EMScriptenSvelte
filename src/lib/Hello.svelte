<script lang="ts">
  import { onDestroy, onMount } from "svelte";
  import create_module from "./hello.js";
  import Editor from "./editor/Editor.svelte";
  let hello: any;

  export let message = "";
  let count = 0;

  let canvas: HTMLCanvasElement;
  let ctx: CanvasRenderingContext2D;

  let cursors = {
    x: 0,
    y: 0,
  };

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

  function toRGB(color: number): string {
    var R = (color >> 8) & 0xff;
    var G = (color >> 3) & 0xff;
    var B = (color & 0x1f) << 3;

    return "rgb(" + R + ", " + G + ", " + B + ")";
  }

  const onCall = (ev: Event) => {
    const details: OnCall = (ev as CustomEvent).detail;
    console.log("onCall:", details.function, " - ", details.args);

    if (details.function === "Debug_Printf") {
      message = details.args.output;

      ctx.fillStyle = "white";
      ctx.fillRect(
        details.args.x * 6,
        details.args.y * 12,
        message.length * 6,
        12
      );

      ctx.font = "12px monospace";
      ctx.fillStyle = "black";
      ctx.fillText(message, details.args.x * 6, (details.args.y + 1) * 12 - 2);
    } else if (details.function === "fillScreen") {
      const { color } = details.args;
      console.log("color", color);

      ctx.fillStyle = toRGB(color);
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    } else if (details.function === "Debug_PrintString") {
      message = details.args.string;

      ctx.fillStyle = "white";
      ctx.fillRect(cursors.x * 12, cursors.y * 24, message.length * 12, 24);

      ctx.font = "24px monospace";
      ctx.fillStyle = "black";
      ctx.fillText(message, cursors.x * 12, (cursors.y + 1) * 24 - 4);
    } else if (details.function === "Debug_SetCursorPosition") {
      const { x, y } = details.args;
      cursors = {
        x,
        y,
      };
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

	fillScreen(color(0,64,0));

  Debug_Printf(10,1,false,0,"HelloWorld%d",42);

  Debug_SetCursorPosition(13,1);
	Debug_PrintString("HelloWorld",0);

  calcEnd(); //restore screen and do stuff

  return 0;
}`;
</script>

<div class="container">
  <div class="left">
    <Editor bind:code />
    <!-- <textarea
      name="code"
      id="code"
      cols="30"
      rows="40"
      bind:value={code}
      readonly
    /> -->
  </div>
  <div class="right">
    <!-- <span>hello._add(4,5): {count}</span> -->
    <!-- <p>
      Debug_Printf:
      <code>{message}</code>
    </p> -->

    <div class="cp400">
      <span class="brand">CP400</span>
      <canvas width="320" height="528" bind:this={canvas} />
    </div>
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
    min-height: 100%;
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
  .cp400 {
    padding: 32px 12px;
    border-radius: 32px / 16px;
    background-color: #121212;
    padding-bottom: 64px;
    position: relative;
  }

  .cp400 .brand {
    position: absolute;
    top: 4px;
    left: 0;
    width: 100%;
    user-select: none;
    opacity: 0.2;
    text-align: center;
  }
</style>
