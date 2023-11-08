/// <reference types="emscripten" />

Module["onRuntimeInitialized"] = function () {
  console.log("Emscripten Ready!");
  document.dispatchEvent(new Event("emReady"));

  Module["add"] = cwrap("add", "number", ["number", "number"]);
  // Debug_printf(int x, int y, const char *message)
  Module["Debug_printf"] = cwrap("Debug_printf", null, [
    "number",
    "number",
    "string",
  ]);
  Module._main();
};
