/// <reference types="emscripten" />

Module["onRuntimeInitialized"] = function () {
  Module["add"] = cwrap("add", "number", ["number", "number"]);
  // Debug_printf(int x, int y, const char *message)
  Module["add"] = cwrap("Debug_printf", null, ["number", "number", "string"]);
};
