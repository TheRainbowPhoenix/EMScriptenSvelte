/// <reference types="emscripten" />

var Module = {
  // print: function (text) {
  //   alert("stdout: " + text);
  // },
  // printErr: function (text) {
  //   alert("stderr: " + text);
  // },
  Debug_printf: function (x, y, message) {
    console.log(`[x: ${x}, y: ${y}] "${message}"`);
  },
};
