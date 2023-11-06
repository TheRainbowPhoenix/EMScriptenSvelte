#include <emscripten.h>
#include <stdio.h>

#include <appdef.hpp>
#include <sdk/test.h>
#include <sdk/calc/calc.hpp>
#include <sdk/os/debug.hpp> // Debug_printf

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("My app name")
APP_DESCRIPTION("A short description of my app")
APP_AUTHOR("My name")
APP_VERSION("1.0.2")

extern "C" {

// EMSCRIPTEN_KEEPALIVE
// void Debug_printf(int x, int y, const char *message) {
//   emscripten_run_script("console.log(\"Debug_printf called\")");
//   printf("[STUB - from printf]\n");
// }

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
  printf("[add %d %d]\n", a, b);
  return a + b;
}

int main() {
  calcInit(); //backup screen and init some variables


  Debug_Printf(10,1,false,0,"HelloWorld%d",42);

  // Debug_printf(0, 1, "hello world");
  int a = add(1, 2);
  // Debug_printf(a, 0, "add");
  printf("My constant is: %d\n", MY_CONSTANT);

  calcEnd(); //restore screen and do stuff

  return 0;
}

} // extern "C"