:: You'll need EMScripten for that to be activated
::  -sWASM=0

emcc -I sdk hello.cpp -O3 -s ENVIRONMENT='web' -o ../src/lib/hello.js  -s EXPORT_ES6=1 -s MODULARIZE=1 -s ENVIRONMENT='web' -s NO_DISABLE_EXCEPTION_CATCHING -s EXPORTED_RUNTIME_METHODS=["cwrap","run","UTF8ToString"] --bind --pre-js hello.pre.js --post-js hello.post.js --extern-pre-js hello.pre.js  -s ASSERTIONS=0 --js-library sdk.js