call ..\EMScripten\emsdk\emsdk_env.bat

cd wasm
build-all.bat
cd ..

pnpm run build