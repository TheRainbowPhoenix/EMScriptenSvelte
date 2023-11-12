const width = 320;
const height = 528;

addToLibrary({
  Debug_printf: function (x, y, invert, zero, format, ...any) {
    console.log("## Debug_printf ##");
  },
  vram: null,
  width: 320,
  height: 528,
  noInitialRun: true,
  cursor: {
    // TODO: move to the actual memory space maybe ?
    x: 0,
    y: 0,
  },
  debug: {
    drawText: () => {},
  },
});
