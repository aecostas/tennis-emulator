import { useEffect, useRef, useState } from "react";
import "./App.css";

function App() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    let isMounted = true;

    const initWasm = async () => {
      const canvas = canvasRef.current;
      if (!canvas) return;

      const script = document.createElement("script");
      script.src = "/cpp/tennis_emulator.js";
      script.onload = () => {
        const createModule = (window as any).createTennisEmulatorModule;
        createModule({
          canvas: canvas,
          noInitialRun: false, // DEJAR QUE RAYLIB EJECUTE main()
          locateFile: (path: string) => "/cpp/" + path,
          print: (t: string) => console.log("[WASM]", t),
          printErr: (t: string) => console.error("[WASM]", t),
        }).then(() => {
          console.log("WASM listo");
          setIsLoading(false);
        });
      };

      document.body.appendChild(script);
    };

    initWasm();

    // Cleanup
    return () => {
      isMounted = false;
      // No removemos el script para evitar problemas si React recarga el componente
    };
  }, []);

  return (
    <div className="app">
      <h1>Tennis Emulator</h1>
      {isLoading && <p>Cargando WebAssembly...</p>}
      {error && (
        <div className="error-container">
          <p className="error">Error: {error}</p>
          <p className="help-text">
            Asegúrate de haber compilado el código C++ con:
            <br />
            <code>npm run build:wasm</code>
            <br />
            O manualmente:
            <br />
            <code>RAYLIB_PATH=/home/andres/raylib npm run build:wasm:make</code>
            <br />Y luego mover los archivos a public/cpp/ si es necesario
          </p>
        </div>
      )}
      <canvas
        ref={canvasRef}
        id="tennis-emulator-canvas"
        width={800}
        height={600}
        style={{
          border: "1px solid #333",
          display: isLoading || error ? "none" : "block",
        }}
      />
    </div>
  );
}

export default App;
