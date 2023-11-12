#pragma once
#include <emscripten.h>
#include <stdint.h>
#include <stdio.h>

#include <sdk/os/lcd.hpp> //needed for getVramAddress and 


//Graphics stuff

extern uint16_t *vram;  //The vram pointer (this is used by routines like setPixel and has to be initialized by getVramAddress() or calc_init();
extern int width;	//width  of the screen
extern int height;	//height of the screen

void setPixel(int x,int y, uint32_t color);

void line(int x1, int y1, int x2, int y2, uint16_t color) {
	int8_t ix, iy;

	int dx = (x2>x1 ? (ix=1, x2-x1) : (ix=-1, x1-x2) );
	int dy = (y2>y1 ? (iy=1, y2-y1) : (iy=-1, y1-y2) );

	setPixel(x1,y1,color);
	if(dx>=dy){ //the derivative is less than 1 (not so steep)
		//y1 is the whole number of the y value
		//error is the fractional part (times dx to make it a whole number)
		// y = y1 + (error/dx)
		//if error/dx is greater than 0.5 (error is greater than dx/2) we add 1 to y1 and subtract dx from error (so error/dx is now around -0.5) 
		int error = 0;
		while (x1!=x2) {
			x1 += ix; //go one step in x direction
			error += dy;//add dy/dx to the y value.
			if (error>=(dx>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				y1+=iy;
				error-=dx;
			}
			setPixel(x1,y1,color);
		}
	}else{ //the derivative is greater than 1 (very steep)
		int error = 0;
		while (y1!=y2) { //The same thing, just go up y and look at x
			y1 += iy; //go one step in y direction
			error += dx;//add dx/dy to the x value.
			if (error>=(dy>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				x1+=ix;
				error-=dy;
			}
			setPixel(x1,y1,color);
		}
	}
}

void vline(int x, int y1, int y2, uint16_t color);

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t colorFill, uint16_t colorLine){
//Filled triangles are a lot of vertical lines.
/*                                                               -
                        a   ___________----------P3              -
       P0 _________---------              ____---                -
          ---____               _____-----                       -
               b ----___  _-----   c                             -
                        P2                                       -
The triangle has three points P0, P1 and P2 and three lines a, b and c. We go from left to right, calculating the point on a and the point on b or c and then we draw a vertical line connecting these two.
*/
//Sort the points by x coordinate
	{int z;
	if(x0>x2){ z=x2; x2=x0; x0=z; z=y2; y2=y0; y0=z; }
	if(x1>x2){ z=x2; x2=x1; x1=z; z=y2; y2=y1; y1=z; }
	if(x0>x1){ z=x1; x1=x0; x0=z; z=y1; y1=y0; y0=z; }}

	int x = x0; //x is the variable that counts from left to right

	//Values for line a
	int ay = y0; //The point y for the current x on the line a
	int aiy; //The direction of line a
	int adx = (x2>x0 ? (       x2-x0) : (        x0-x2) );
	int ady = (y2>y0 ? (aiy=1, y2-y0) : (aiy=-1, y0-y2) );
	int aerr = 0; //The y value of a (fractional part). y is actually ay+(aerr/adx)

	//Values for line b
	int by = y0; //The point y for the current x on the line b
	int biy; //The direction of line b
	int bdx = (x1>x0 ? (       x1-x0) : (        x0-x1) );
	int bdy = (y1>y0 ? (biy=1, y1-y0) : (biy=-1, y0-y1) );
	int berr = 0;

	//Values for line c
	int cy = y1; //The point y for the current x on the line y (starting at P1)
	int ciy; //The direction of line c
	int cdx = (x2>x1 ? (       x2-x1) : (        x1-x2) );
	int cdy = (y2>y1 ? (ciy=1, y2-y1) : (ciy=-1, y1-y2) );
	int cerr = 0;

	//First draw area between a and b
	while (x<x1){
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		berr+=bdy;
		while(berr>=bdx >> 2){ //if berr/bdx >= 0.5
			berr-=bdx;
			by+=biy;
		}
		vline(x,ay,by,colorFill);
	}

	//Then draw area between a and c
	while (x<x2-1){ //we don't need x=x2
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		cerr+=cdy;
		while(cerr>=cdx >> 2){ //if berr/bdx >= 0.5
			cerr-=cdx;
			cy+=ciy;
		}
		vline(x,ay,cy,colorFill);
	}

	line(x0,y0,x1,y1,colorLine);
	line(x1,y1,x2,y2,colorLine);
	line(x2,y2,x0,y0,colorLine);
}

void vline(int x, int y1, int y2, uint16_t color){
	if (y1>y2) { int z=y2; y2=y1; y1=z;}
	for (int y=y1; y<=y2; y++)
		setPixel(x,y,color);
}


void fillScreen(uint16_t color) {
	
	uint16_t custom_color = color;

	EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
		const size = Module.width * Module.height;
		var vramArray = new Uint16Array(Module.vram.buffer, 0, size);
		vramArray.fill($0);

		document.dispatchEvent(new CustomEvent("onCall", { detail: {
            function: "fillScreen",
            args: {
                color: $0
            }
        } }));
    }, custom_color);
}

inline uint16_t color(uint8_t R, uint8_t G, uint8_t B){
	return	(((R<<8) & 0b1111100000000000) |
		 ((G<<3) & 0b0000011111100000) |
		 ((B>>3) & 0b0000000000011111));
}

inline void setPixel(int x,int y, uint32_t color) {
	// int width = EM_ASM_INT({
    //     return Module.width;
    // });

    // int height = EM_ASM_INT({
    //     return Module.height;
    // });

	 EM_ASM_({
        var x = $0;
        var y = $1;
        var color = $2;

        // Use Module.width and Module.height
        var width = Module.width;
        var height = Module.height;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            // Calculate the index in the vram array
            var index = y * width + x;

            // Set the pixel color directly in Module.vram
            Module.vram[index] = color;

            // document.dispatchEvent(new CustomEvent("onCall", {
            //     detail: {
            //         function: "setPixel",
            //         args: {
            //             x: x,
            //             y: y,
            //             color: color,
            //         },
            //     },
            // }));
        }
    }, x, y, color);
}

//Stuff for Initialisation and stuff

EMSCRIPTEN_KEEPALIVE
inline void calcInit(){
	emscripten_run_script("console.log(\"calcInit called - emscripten_run_script\")");
	emscripten_run_script("document.dispatchEvent(new CustomEvent(\"calcInit\", { detail: \"test emscripten_run_script\" }))");
	printf("[STUB - calcInit]\n");

	int msgbuf = 42;
	// vram = LCD_GetVRAMAddress();
	// LCD_GetSize(&width, &height);
	// LCD_VRAMBackup();

	EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
		console.log("calcInit - EM_ASM",  $0);	

		document.dispatchEvent(new CustomEvent("calcInit", { detail: `test EM_ASM ${$0}` }));
    }, msgbuf);
}

EMSCRIPTEN_KEEPALIVE
inline void calcEnd(){
	emscripten_run_script("console.log(\"calcEnd called\")");
  printf("[STUB - calcEnd]\n");
	// LCD_VRAMRestore();
	// LCD_Refresh();

	// EM_ASM_({ // Use EM_ASM_ to execute JavaScript code from C
    //     var event = new CustomEvent("calcEnd", { detail: Module.Pointer_stringify($0) });
    //     document.dispatchEvent(event);
    // }, "calcEnd called");
	EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
		console.log("calcEnd - EM_ASM");	

		document.dispatchEvent(new CustomEvent("calcEnd", { detail: `test EM_ASM` }));
    });
}

void LCD_Refresh() {
	EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
		document.dispatchEvent(new CustomEvent("onCall", { detail: {
            function: "LCD_Refresh",
            args: {}
        } }));
    });
}

//Stuff for the keyboard

extern "C" void getKey(uint32_t *key1, uint32_t *key2);

enum Keys1 {
	KEY_SHIFT		= 0x80000000,
	KEY_CLEAR		= 0x00020000, //The Power key
	KEY_BACKSPACE		= 0x00000080,
	KEY_LEFT		= 0x00004000,
	KEY_RIGHT		= 0x00008000,
	KEY_Z			= 0x00002000,
	KEY_POWER		= 0x00000040, //The exponent key
	KEY_DIVIDE		= 0x40000000,
	KEY_MULTIPLY		= 0x20000000,
	KEY_SUBTRACT		= 0x10000000,
	KEY_ADD			= 0x08000000,
	KEY_EXE			= 0x04000000,
	KEY_EXP			= 0x00000004,
	KEY_3			= 0x00000008,
	KEY_6			= 0x00000010,
	KEY_9			= 0x00000020,
};

enum Keys2 {
	KEY_KEYBOARD		= 0x80000000,
	KEY_UP			= 0x00800000,
	KEY_DOWN		= 0x00400000,
	KEY_EQUALS		= 0x00000080,
	KEY_X			= 0x00000040,
	KEY_Y			= 0x40000000,
	KEY_LEFT_BRACKET	= 0x00000020,
	KEY_RIGHT_BRACKET	= 0x00000010,
	KEY_COMMA		= 0x00000008,
	KEY_NEGATIVE		= 0x00000004,
	KEY_0			= 0x04000000,
	KEY_DOT			= 0x00040000,
	KEY_1			= 0x08000000,
	KEY_2			= 0x00080000,
	KEY_4			= 0x10000000,
	KEY_5			= 0x00100000,
	KEY_7			= 0x20000000,
	KEY_8			= 0x00200000,
};

inline bool testKey(uint32_t key1, uint32_t key2, Keys1 key){
	(void) key2;
	if (key1 & key) return true;
	else return false;
}

inline bool testKey(uint32_t key1, uint32_t key2, Keys2 key){
	(void) key1;
	if (key2 & key) return true;
	else return false;
}
