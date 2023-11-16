:: call ..\EMScripten\emsdk\emsdk_env.bat

echo "Building ..." > dist/preview.html

cd wasm
call build-all.bat
cd ..

call pnpm run build

copy /B dist\panel.html dist\preview.html

echo. > dist/.index