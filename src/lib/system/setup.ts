import {charmap, small_map} from "./font"

export const setup = (globalThis: any) => {
    if(!globalThis.fonts) {
        globalThis.fonts = {}
    } 
    globalThis.fonts.getLetter = (x: string) => {

        if (small_map[x]) {
            return [
                0, 0, 0, 0, 0,
                ...small_map[x].data,
            ]
        }

        return [
            0, 0, 0, 0, 0, // added
            1, 1, 1, 1, 1, 
			1, 0, 0, 0, 1, 
			1, 0, 0, 0, 1, 
			1, 0, 0, 0, 1, 
			1, 0, 1, 0, 1,

			1, 0, 1, 0, 1, 
			1, 0, 0, 0, 1, 
			1, 0, 0, 0, 1, 
			1, 0, 0, 0, 1, 
			1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, // added
		]
    }
}