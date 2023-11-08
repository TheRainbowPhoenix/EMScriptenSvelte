<script lang="ts">
  //   import type monaco from "monaco-editor";
  import { onMount } from "svelte";
  //   import editorWorker from "monaco-editor/esm/vs/editor/editor.worker?worker";
  //   import jsonWorker from "monaco-editor/esm/vs/language/json/json.worker?worker";
  //   import cssWorker from "monaco-editor/esm/vs/language/css/css.worker?worker";
  //   import htmlWorker from "monaco-editor/esm/vs/language/html/html.worker?worker";
  //   import tsWorker from "monaco-editor/esm/vs/language/typescript/ts.worker?worker";
  //   import tsWorker from "monaco-editor/esm/vs/basic-language/cpp/cpp.js?worker";
  //   import { create } from "./hhkLang";

  import * as monaco from "monaco-editor/esm/vs/editor/editor.api";
  import "monaco-editor/esm/vs/basic-languages/cpp/cpp.contribution";
  import { defineTheme } from "./theme";

  let divEl: HTMLDivElement = null;
  let editor: monaco.editor.IStandaloneCodeEditor;
  let Monaco;

  export let code: string = [
    "function x() {",
    '\tconsole.log("Hello world!");',
    "}",
  ].join("\n");

  onMount(async () => {
    // monaco.languages.register({ id: 'cpp' });

    defineTheme();

    editor = monaco.editor.create(divEl, {
      theme: "one-dark",
      value: code,
      language: "cpp",
      lightbulb: {
        enabled: true,
      },
      fontWeight: "500",
      minimap: {
        enabled: true,
      },
    });

    return () => {
      editor.dispose();
    };
  });
</script>

<div bind:this={divEl} class="h-screen" />

<style>
  .h-screen {
    min-height: 100%;
  }
</style>
