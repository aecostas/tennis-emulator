import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    headers: {
      "Cross-Origin-Opener-Policy": "same-origin",
      "Cross-Origin-Embedder-Policy": "require-corp",
    },
  },
  optimizeDeps: {
    exclude: ["src/cpp/tennis_emulator.js"],
  },
  build: {
    sourcemap: false, // Desactivar source maps en producción
  },
  // Suprimir advertencias de source maps en desarrollo
  logLevel: "warn",
});
