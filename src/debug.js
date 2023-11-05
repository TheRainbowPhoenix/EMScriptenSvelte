// Define the global Debug_printf function
/**
 *
 * @param {number} x
 * @param {number} y
 * @param {string} message
 */
globalThis.Debug_printf = (x, y, message) => {
  console.log(`[x: ${x}, y: ${y}] "${message}"`);
};
