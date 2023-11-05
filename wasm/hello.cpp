#include <emscripten.h>
#include <stdio.h>

extern "C" {

EMSCRIPTEN_KEEPALIVE
void Debug_printf(int x, int y, const char *message) {
  emscripten_run_script("console.log(\"Debug_printf called\")");
  printf("[STUB - from printf]\n");
}

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
  printf("[add %d %d]\n", a, b);
  return a + b;
}

int main() {
  Debug_printf(0, 1, "hello world");
  int a = add(1, 2);
  Debug_printf(a, 0, "add");
  return 0;
}

} // extern "C"