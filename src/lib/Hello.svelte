<script lang="ts">
  import { onDestroy, onMount } from "svelte";
  // import Editor from "./editor/Editor.svelte";
  import { setup } from "./system/setup.js";
  export let ide = true;

  let hello: any;
  let Editor: any;

  export let message = "";
  let count = 0;

  let canvas: HTMLCanvasElement;
  let ctx: CanvasRenderingContext2D;

  let cursors = {
    x: 0,
    y: 0,
  };

  onMount(async () => {
    setup(window);

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

    document.addEventListener("jsReady", doReady);
    document.addEventListener("calcInit", onCalcInit);
    document.addEventListener("calcEnd", onCalcEnd);
    document.addEventListener("onCall", onCall);

    // hello = await create_module();
    // @ts-ignore
    if (window.hello) {
      // @ts-ignore
      hello = window.hello;
      hello._runMain();
    }

    if (ide) {
      // Editor = (await import('./editor/Editor.svelte')).default;
    }

    // hello._runMain();
    // console.log(hello.getVRAM);
    // count = hello._add(4, 5);
    // hello.Debug_printf(10, 2, false, 0, "Hello from Svelte (JS wrapper)");
  });

  const doReady = (ev: CustomEvent) => {
    // @ts-ignore
    hello = window.hello;
    // @ts-ignore
    console.debug("module", window.hello);
    // @ts-ignore
    window.hello._runMain();
  }

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

  function rgb565ToRgb(color565: number): [number, number, number] {
    const R = ((color565 >> 11) & 0x1f) << 3;
    const G = ((color565 >> 5) & 0x3f) << 2;
    const B = (color565 & 0x1f) << 3;
    return [R, G, B];
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
    } else if (details.function === "setPixel") {
      const { x, y, color } = details.args;
      console.log("setPixel", x, y, color);
    } else if (details.function === "fillScreen") {
      const { color } = details.args;
      console.log("color", color);

      ctx.fillStyle = toRGB(color);
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    } else if (details.function === "LCD_Refresh") {
      console.log("## LCD_Refresh ##");

      const vram = hello.getVRAM();
      const { width, height } = hello;

      for (let y = 0; y < height; y++) {
        for (let x = 0; x < width; x++) {
          const index = y * width + x;
          const color565 = vram[index];
          const [r, g, b] = rgb565ToRgb(color565);

          // Set pixel on the canvas
          ctx.fillStyle = `rgb(${r},${g},${b})`;
          ctx.fillRect(x, y, 1, 1);
        }
      }

      console.log(vram);
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

  const calcToolbar = [
    {
      name: "Settings",
    },
    {
      name: "Menu",
    },
    {
      name: "Main",
    },
    {
      name: "Resize",
    },
    {
      name: "Swap",
    },
    {
      name: "Rotate",
    },
    {
      name: "Esc",
    },
  ];

  let code = `int main() {
	calcInit(); //backup screen and init some variables

	fillScreen(color(0,64,0));
	setPixel(1, 0, 0x5678);

  Debug_Printf(10,1,false,0,"HelloWorld%d",42);

  Debug_SetCursorPosition(13,1);
  Debug_PrintString("HelloWorld",0);

  //Example for setPixel(x,y,color)
  for (int x=0; x<256;x++){
    for (int y=0; y<256; y++){
      setPixel(50+x,250+y, color(x,y,0) );
    }
  }

  //Example for triangle(x0,y0,x1,y1,x2,y2,colorFill,colorLine);
  triangle(10,20,40,250,300,100,color(0,255,0),color(0,0,255));

  //Example for line(x1,y1,x2,y2,color);
  line(100,30,290,500,color(255,0,0));      //Use RGB color
  line(110,30,300,500,0b1111100000000000);  //Or use 565 color

	//Don't forget to do LCD_Refresh after setPixel(); line(); and triangle();
	LCD_Refresh();

  calcEnd(); //restore screen and do stuff

  return 0;
}`;
</script>

<div class="container">
  {#if ide}
    
  <div class="left">
    <!-- <Editor bind:code /> -->
    <!-- <textarea
      name="code"
      id="code"
      cols="30"
      rows="40"
      bind:value={code}
      readonly
    /> -->
  </div>
  
  {/if}
  <div class="right">
    <!-- <span>hello._add(4,5): {count}</span> -->
    <!-- <p>
      Debug_Printf:
      <code>{message}</code>
    </p> -->

    <div class="cp400">
      <div class="cp-inner">
        <span class="brand">CP400</span>
        <div class="pre-screen">
          <canvas width="320" height="528" bind:this={canvas} />
          <div class="toolbar">
            {#each calcToolbar as ctool}
              <button>
                <span class="text">{ctool.name}</span>
                <span class="icon" />
              </button>
            {/each}
          </div>
        </div>
      </div>
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
    /* flex-basis: 450px; */
  }
  canvas {
    width: 320px;
    height: 528px;
    background-color: black;
  }

  .cp400 {
    padding: 8px 4px;
    border-radius: 32px / 16px;
    background-color: #e8e8e8;
  }

  .cp400 .cp-inner {
    border: 2px solid #1e7999;
    padding: 32px 6px;
    border-radius: 24px / 32px;
    background-color: #121212;
    padding-bottom: 64px;
    position: relative;
  }

  .cp400 .pre-screen {
    border: 8px solid #0a0a0a;
    border-radius: 8px;
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

  .cp400 .toolbar {
    display: grid;
    grid-template-columns: repeat(7, 1fr);
    gap: 4px;
    max-width: 320px;
  }

  .cp400 .toolbar button {
    padding: 0;
    padding-bottom: 4px;
    text-align: center;
    overflow: hidden;

    display: flex;
    gap: 4px;
    flex-direction: column;
    align-items: center;

    background-color: transparent;
  }

  .cp400 .toolbar button span.text {
    font-size: 10px;
    color: rgba(255, 255, 255, 0.3);
  }

  .cp400 .toolbar button span.icon {
    width: 24px;
    height: 24px;
    background-color: rgba(255, 255, 255, 0.1);
  }
</style>
