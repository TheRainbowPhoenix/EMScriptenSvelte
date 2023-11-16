import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [svelte()],
  build: {
    rollupOptions: {
      output: {
        entryFileNames: 'js/[name].js', // JavaScript bundles
        chunkFileNames: 'js/chunk-[name].js', // Code-split chunks
        assetFileNames: 'assets/[name].[ext]' // Asset files (images, fonts, etc.)
      }
    }
  }
})
