/// <reference types="emscripten" />

export interface HelloModule extends EmscriptenModule {
  add(a: number, b: number): number;
  Debug_printf(x: number, y: number, message: string): void;
}
