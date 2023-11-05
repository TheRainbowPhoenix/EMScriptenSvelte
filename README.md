# Svelte + EMScripten

## Recommended IDE Setup

[VS Code](https://code.visualstudio.com/) + [Svelte](https://marketplace.visualstudio.com/items?itemName=svelte.svelte-vscode).

## EMScripten setup

Go follow the [EMScripten Install Guide](https://emscripten.org/docs/getting_started/downloads.html)

Bascially the TLDR (on Windows):

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
emsdk install latest
emsdk activate latest
emsdk_env.bat

cd ..\wasm
build.bat
```

## Run the project

```
npm i     # or pnpm i
npm run dev
```

Open in your browser : [http://localhost:5173/](http://localhost:5173/)
