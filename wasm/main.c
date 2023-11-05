#include <emscripten.h>
#include <stdio.h>

extern "C" {

EMSCRIPTEN_KEEPALIVE
void Debug_printf(int x, int y, const char *message) {
  emscripten_run_script("console.log(\"Hello\")");
  printf("[STUB !!]\n");
}

int main() {
  Debug_printf(0, 1, "hello world");
  return 0;
}

} // extern "C"