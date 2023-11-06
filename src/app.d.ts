declare global {
  namespace App {
    // interface Error {}
    // interface Locals {}
    // interface PageData {}
    // interface Platform {}
  }

  declare namespace svelteHTML {
    interface Document {
      "on:calcInit"?: CustomEvent<T>;
      "on:calcEnd"?: CustomEvent<T>;
    }
  }
}
