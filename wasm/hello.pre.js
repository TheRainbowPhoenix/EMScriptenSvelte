/// <reference types="emscripten" />

var Module = {
  vram: null,
  width: 320,
  height: 528,
  // print: function (text) {
  //   alert("stdout: " + text);
  // },
  // printErr: function (text) {
  //   alert("stderr: " + text);
  // },
  Debug_printf: function (x, y, message) {
    console.log(`[x: ${x}, y: ${y}] "${message}"`);
  },
  calcInit: function () {
    console.log("calcInit");
  },
  preInit: function () {
    console.debug("-- EMS: preInit");
    // Initialize vram, width, and height
    Module.vram = new Uint16Array(Module.width * Module.height);
  },
  postInit: function () {
    // Additional post-init code if needed
  },
  onRuntimeInitialized: function () {
    // Additional initialization code after runtime is initialized
  },
  getVRAM: function () {
    return Module.vram;
  },
};
