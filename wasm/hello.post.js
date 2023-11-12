/// <reference types="emscripten" />

function drawText(msg, x, y, invert = false, scale = 1) {
  x = x * 6;
  y = y * 12;
  // leading zeroes
  const pre_color = invert ? 0x0000 : 0xffff;

  for (var row = 0; row < 12 * scale; row++) {
    for (var col = 0; col < 1 * scale; col++) {
      var index = (y * scale + row) * Module.width + x * scale + col;
      Module.vram[index] = pre_color;
    }
  }

  // for each character c
  for (var i = 0; i < msg.length; i++) {
    var letter = globalThis?.fonts?.getLetter(msg.charAt(i));
    if (letter) {
      for (var row = 0; row < 12 * scale; row++) {
        for (var col = 0; col < 6 * scale; col++) {
          var pixelValue =
            Math.floor(col / scale) == 5
              ? 0
              : letter[Math.floor(row / scale) * 5 + Math.floor(col / scale)];
          var color = invert
            ? pixelValue
              ? 0xffff
              : 0x0000
            : pixelValue
            ? 0x0000
            : 0xffff;
          var index =
            (y * scale + row) * Module.width +
            ((x + 1) * scale + col + i * scale * 6);
          Module.vram[index] = color;
        }
      }
    }
  }

  // ending zeroes
  for (var row = 0; row < 12 * scale; row++) {
    for (var col = 0; col < 1 * scale; col++) {
      var index =
        (y * scale + row) * Module.width +
        (x * scale + 1 * scale + msg.length * 6 * scale + col);
      Module.vram[index] = pre_color;
    }
  }
}

Module["onRuntimeInitialized"] = function () {
  document.dispatchEvent(new Event("emReady"));

  Module["add"] = cwrap("add", "number", ["number", "number"]);
  // Debug_printf(int x, int y, const char *message)
  Module["Debug_printf"] = cwrap("Debug_printf", null, [
    "number",
    "number",
    "string",
  ]);
};

Module["debug"] = Module["debug"] || {};

Module["debug"]["drawText"] = drawText;
