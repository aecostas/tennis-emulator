import { useEffect, useRef, useState } from "react";
import "./App.css";

function App() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const wasmModuleRef = useRef<any>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [angle, setAngle] = useState(0); // Ángulo horizontal en grados
  const [elevation, setElevation] = useState(-20); // Ángulo vertical en grados
  const [speed, setSpeed] = useState(1500); // Velocidad inicial

  useEffect(() => {
    const initWasm = async () => {
      const canvas = canvasRef.current;
      if (!canvas) {
        console.error("Canvas no encontrado");
        return;
      }

      // Asegurarse de que el canvas esté en el DOM y visible
      canvas.id = "tennis-emulator-canvas";
      canvas.style.display = "block";

      // Esperar un frame para asegurar que el canvas esté completamente en el DOM
      await new Promise((resolve) => setTimeout(resolve, 100));

      const script = document.createElement("script");
      script.src = "/cpp/tennis_emulator.js";
      script.onload = () => {
        const createModule = (window as any).createTennisEmulatorModule;
        if (!createModule) {
          console.error("createTennisEmulatorModule no encontrado");
          setIsLoading(false);
          return;
        }

        console.log("Inicializando módulo WASM con canvas:", canvas.id);
        createModule({
          canvas: canvas,
          noInitialRun: true, // Ejecutar main() manualmente después
          locateFile: (path: string) => "/cpp/" + path,
          print: (t: string) => console.log("[WASM]", t),
          printErr: (t: string) => console.error("[WASM]", t),
        })
          .then((module: any) => {
            console.log("WASM listo", module);
            wasmModuleRef.current = module;

            // Ejecutar main() manualmente después de que el módulo esté listo
            if (module._main) {
              console.log("Ejecutando main()...");
              try {
                module._main();
              } catch (err) {
                console.error("Error ejecutando main():", err);
              }
            } else {
              console.warn("_main no encontrado en el módulo");
            }

            setIsLoading(false);
          })
          .catch((err: any) => {
            console.error("Error cargando WASM:", err);
            setIsLoading(false);
          });
      };

      script.onerror = () => {
        console.error("Error cargando script WASM");
        setIsLoading(false);
      };

      document.body.appendChild(script);
    };

    initWasm();

    // Cleanup
    return () => {
      // No removemos el script para evitar problemas si React recarga el componente
    };
  }, []);

  const handleSetAngle = () => {
    if (wasmModuleRef.current && wasmModuleRef.current._setBallAngle) {
      wasmModuleRef.current._setBallAngle(angle, elevation, speed);
    } else {
      console.warn("WASM module not ready yet");
    }
  };

  const handleShootBall = () => {
    if (wasmModuleRef.current) {
      // Primero actualizar el ángulo y velocidad
      if (wasmModuleRef.current._setBallAngle) {
        wasmModuleRef.current._setBallAngle(angle, elevation, speed);
      }
      // Luego disparar
      if (wasmModuleRef.current._shootBall) {
        wasmModuleRef.current._shootBall();
      }
    } else {
      console.warn("WASM module not ready yet");
    }
  };

  return (
    <div className="app">
      <h1>Tennis Emulator</h1>
      {isLoading && <p>Cargando WebAssembly...</p>}
      {!isLoading && (
        <div
          style={{
            marginBottom: "10px",
            padding: "10px",
            backgroundColor: "#f0f0f0",
            borderRadius: "4px",
          }}
        >
          <div style={{ marginBottom: "10px" }}>
            <label style={{ display: "block", marginBottom: "5px" }}>
              Ángulo Horizontal: {angle}°
              <input
                type="range"
                min="-90"
                max="90"
                value={angle}
                onChange={(e) => setAngle(Number(e.target.value))}
                style={{ width: "200px", marginLeft: "10px" }}
              />
            </label>
            <label style={{ display: "block", marginBottom: "5px" }}>
              Ángulo Vertical: {elevation}°
              <input
                type="range"
                min="-45"
                max="45"
                value={elevation}
                onChange={(e) => setElevation(Number(e.target.value))}
                style={{ width: "200px", marginLeft: "10px" }}
              />
            </label>
            <label style={{ display: "block", marginBottom: "10px" }}>
              Velocidad: {speed}
              <input
                type="range"
                min="500"
                max="3000"
                step="100"
                value={speed}
                onChange={(e) => setSpeed(Number(e.target.value))}
                style={{ width: "200px", marginLeft: "10px" }}
              />
            </label>
          </div>
          <button
            onClick={handleShootBall}
            style={{
              padding: "10px 20px",
              fontSize: "16px",
              backgroundColor: "#4CAF50",
              color: "white",
              border: "none",
              borderRadius: "4px",
              cursor: "pointer",
            }}
            onMouseOver={(e) => {
              e.currentTarget.style.backgroundColor = "#45a049";
            }}
            onMouseOut={(e) => {
              e.currentTarget.style.backgroundColor = "#4CAF50";
            }}
          >
            Disparar Pelota
          </button>
        </div>
      )}
      <canvas
        ref={canvasRef}
        id="tennis-emulator-canvas"
        width={800}
        height={600}
        style={{
          border: "1px solid #333",
          display: "block",
        }}
      />
    </div>
  );
}

export default App;
