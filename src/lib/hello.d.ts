/// <reference types="emscripten" />

export interface HelloModule extends EmscriptenModule {
  _add(a: number, b: number): number;
  Debug_printf(
    x: number,
    y: number,
    invert: boolean,
    zero: number,
    format: string,
    ...args
  ): void;
}

export default function create_module(mod?: any): Promise<HelloModule>;
