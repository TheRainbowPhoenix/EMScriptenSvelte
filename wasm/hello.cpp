#include <emscripten.h>
#include <stdio.h>

#include <appdef.hpp>
#include <sdk/test.h>
#include <sdk/calc/calc.hpp>
#include <sdk/os/debug.hpp> // Debug_printf
#include <sdk/os/lcd.hpp>

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

	fillScreen(color(0,64,0));
	setPixel(1, 0, 0x5678);

	Debug_Printf(10,1,false,0,"HelloWorld%d",42);

	Debug_SetCursorPosition(13,1);
	Debug_PrintString("Hello World !",0);

	LCD_Refresh();

	// //Example for setPixel(x,y,color)
	for (int x=0; x<256;x++){
		for (int y=0; y<256; y++){
			setPixel(50+x,250+y, color(x,y,0) );
		}
	}

	// //Example for triangle(x0,y0,x1,y1,x2,y2,colorFill,colorLine);
	triangle(10,20,40,250,300,100,color(0,255,0),color(0,0,255));

	// //Example for line(x1,y1,x2,y2,color);
	line(100,30,290,500,color(255,0,0));      //Use RGB color
	line(110,30,300,500,0b1111100000000000);  //Or use 565 color

	// //Don't forget to do LCD_Refresh after setPixel(); line(); and triangle();
	LCD_Refresh();

	// Debug_printf(0, 1, "hello world");
	int a = add(1, 2);
	// Debug_printf(a, 0, "add");
	printf("My constant is: %d\n", MY_CONSTANT);

	calcEnd(); //restore screen and do stuff

	return 0;
}

} // extern "C"