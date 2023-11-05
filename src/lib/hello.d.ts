/// <reference types="emscripten" />

export interface HelloModule extends EmscriptenModule {
  _add(a: number, b: number): number;
  _Debug_printf(x: number, y: number, message: string): void;
  Debug_printf(x: number, y: number, message: string): void;
}

export default function create_module(mod?: any): Promise<HelloModule>;
