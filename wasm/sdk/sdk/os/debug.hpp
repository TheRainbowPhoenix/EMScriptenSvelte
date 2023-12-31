/**
 * @file
 * @brief Functions useful during debugging.
 * 
 * It can be useful to print text or numbers to the screen whilst debugging,
 * without creating a full GUI, or pause execution and wait for a key press.
 * Thoroughly recommended only for debugging - prefer GUI elements for
 * user-facing input/output!
 *
 * Example:
 * @code{cpp}
 * // Print "Hello, world!" at 0, 0 in black on white
 * Debug_SetCursorPosition(0, 0);
 * Debug_PrintString("Hello, world!", false);
 *
 * // Print "Inverted text" at 1, 1 in white on black
 * Debug_SetCursorPosition(1, 1);
 * Debug_PrintString("Inverted text", true);
 *
 * // Print the number 0x1322 at 3, 7
 * Debug_PrintNumberHex_Word(0x1322, 3, 7);
 *
 * // Print small text with a format string
 * Debug_Printf(20, 20, false, 0, "Format strings are %s in %d!", "cool", 2018);
 * 
 * // Draw the changes we made to VRAM onto the LCD
 * // Defined in sdk/os/lcd.hpp
 * LCD_Refresh();
 *
 * // Block until the user presses a key
 * Debug_WaitKey();
 * @endcode
 */

#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <emscripten.h>
#include <stdio.h>

/**
 * Returns the current position of the cursor in debug text mode.
 * 
 * @param[out] x,y The position of the cursor.
 */
extern "C"
void Debug_GetCursorPosition(int *x, int *y) {
    EM_ASM({
        Module.cursor = Module.cursor || {};
        setValue($0, Module.cursor.x || 0);
        setValue($1, Module.cursor.y || 0);
    }, x, y);
}

/**
 * Print a formatted string in small debug text mode, either in normal
 * black-on-white or inverted white-on-black. Color inversion occurs if
 * @p invert is true.
 * 
 * Supports most format specifiers.
 * 
 * @param x,y The coordinates to print the formatted string at.
 * @param invert True if the colors used to print the text should be inverted.
 * @param zero Must be passed 0.
 * @param[in] format The format string to use.
 * @param ... The values to substitute into the format string.
 */
extern "C"
void Debug_Printf(int x, int y, bool invert, int zero, const char *format, ...)
 {
    va_list args; // TODO
    va_start(args, format);

    // Calculate the length of the formatted message.
    int len = vsnprintf(nullptr, 0, format, args);

    // Allocate a buffer to store the formatted message.
    char* buffer = new char[len + 1];

    // Format the message into the buffer.
    vsnprintf(buffer, len + 1, format, args);

    va_end(args);

    EM_ASM({
        var msg = Module.UTF8ToString($0);
        var x = $1;
        var y = $2;
        var invert = $3;
        var zero = 0x0000;

        Module.debug.drawText(msg, x, y, invert, 1);
            
        console.log(msg);
    }, buffer, x, y, invert, zero);
    
    // va_start(args, format);

    // va_end(args);

    // https://emscripten.org/docs/api_reference/emscripten.h.html#c.MAIN_THREAD_EM_ASM ?
    EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
		console.log("Debug_Printf - EM_ASM",  $0, $1, $2, $3, Module.UTF8ToString($4));	

		document.dispatchEvent(new CustomEvent("onCall", { detail: {
            function: "Debug_Printf",
            args: {
                x: $0,
                y: $1,
                invert: $2,
                zero: $3,
                output: Module.UTF8ToString($4)
            }
        } }));
    }, x, y, invert, zero, buffer); // TODO: find how to catch variable args

    delete[] buffer;
}


/**
 * Prints the hex representation of a byte (8-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The byte to print.
 * @param x,y The coordinates to print the number at.
 */
extern "C"
void Debug_PrintNumberHex_Byte(uint8_t value, int x, int y);

/**
 * Prints the hex representation of a dword (32-bit number) at the specified
 * position in debug text mode.
 * 
 * @param value The dword to print.
 * @param x,y The coordinates to print the number at.
 */
extern "C"
void Debug_PrintNumberHex_Dword(uint32_t value, int x, int y);

/**
 * Prints the hex representation of a nibble (4-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The nibble to print. High 4 bits are ignored.
 * @param x,y The coordinates to print the number at.
 */
extern "C"
void Debug_PrintNumberHex_Nibble(uint8_t value, int x, int y);

/**
 * Prints the hex representation of a word (16-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The word to print.
 * @param x,y The coordinates to print the number at.
 */
extern "C"
void Debug_PrintNumberHex_Word(uint16_t value, int x, int y);

/**
 * Prints a string in debug text mode, either in normal black-on-white or
 * inverted white-on-black. Color inversion occurs if \c invert is true.
 * 
 * Returns false if the string did not fit on the screen.
 * 
 * @param[in] string The string to print.
 * @param invert True if the colors used to print the text should be inverted.
 * @return True if writing the string was successful, false otherwise.
 */
extern "C"
bool Debug_PrintString(const char *string, bool invert)  {

    const char *ems_string = string;
    bool ems_invert = invert;

    EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
        Module.cursor = Module.cursor || {};

        const msg = Module.UTF8ToString($0);
        const invert = $1;
        const x = Module.cursor.x || 0;
        const y = Module.cursor.y || 0;

        const scale = 2;

        Module.debug.drawText(msg, x, y, invert, scale);


		document.dispatchEvent(new CustomEvent("onCall", { detail: {
            function: "Debug_PrintString",
            args: {
                string: Module.UTF8ToString($0),
                invert: $1
            }
        } }));
    }, ems_string, ems_invert);

    return true;
}

/**
 * Sets the position of the cursor in debug text mode.
 * 
 * @param x,y The cursor's new position.
 * @return Always returns 0.
 */
extern "C"
int Debug_SetCursorPosition(int x, int y) {

    EM_ASM({
        Module.cursor = Module.cursor || {};
        Module.cursor.x = $0;
        Module.cursor.y = $1;
        console.log(Module.cursor);
    }, x, y);

    // EM_ASM({ // Use EM_ASM_ to execute JavaScript code from C
	// 	document.dispatchEvent(new CustomEvent("onCall", { detail: {
    //         function: "Debug_SetCursorPosition",
    //         args: {
    //             x: $0,
    //             y: $1
    //         }
    //     } }));
    // }, ems_x, ems_y);

    return 0;
}

/**
 * Waits until a key is pressed, then returns a number representing the key.
 * Only appears to react to number keys and the Power/Clear key. Returns 0x30
 * to 0x39 for keys 0 to 9, and 0x98 for the Power/Clear key.
 * 
 * @return A number representing the key that was pressed.
 */
extern "C"
int Debug_WaitKey();
