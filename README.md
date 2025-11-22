# Tennis Emulator

Proyecto web con React.js + TypeScript que utiliza C++ y raylib compilado a WebAssembly para renderizar gráficos en un canvas.

## 🎯 Características

- ⚛️ React 19 + TypeScript
- 🚀 Vite como bundler
- 🎨 Raylib para renderizado gráfico
- 🔧 C++ compilado a WebAssembly con Emscripten
- 🖼️ Canvas HTML5 para renderizado
- 👀 **Hot reload automático para C++** (con `dev:watch`)

## 📋 Requisitos Previos

### 1. Emscripten SDK

Emscripten es necesario para compilar C++ a WebAssembly. Instálalo siguiendo estos pasos:

```bash
# Clonar el repositorio de Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Instalar y activar la última versión
./emsdk install latest
./emsdk activate latest

# Activar las variables de entorno (hazlo en cada terminal nueva)
source ./emsdk_env.sh
```

Verifica la instalación:

```bash
emcc --version
```

### 2. Raylib para Web

Raylib necesita estar instalado (clonado) y compilado para WebAssembly. Tienes dos opciones:

**Opción A (Recomendada)**: Dejar que el script lo compile automáticamente

- Solo necesitas clonar raylib: `git clone https://github.com/raysan5/raylib.git`
- El script `npm run build:wasm` detectará si raylib está compilado y te ofrecerá compilarlo automáticamente

**Opción B**: Compilar raylib manualmente primero

```bash
# Clonar raylib
git clone https://github.com/raysan5/raylib.git

# Compilar raylib explícitamente
cd tennis-emulator
RAYLIB_PATH=/ruta/a/raylib npm run build:raylib
```

> **Nota**: El script `build:wasm` ahora detecta automáticamente si raylib necesita compilación y te lo ofrece.

## 🚀 Instalación Rápida

Usa el script de configuración automática:

```bash
chmod +x setup.sh
./setup.sh
```

O sigue los pasos manuales:

### Paso 1: Instalar dependencias de Node.js

```bash
npm install
```

### Paso 2: Compilar el código C++ a WebAssembly

**Importante**: Asegúrate de tener Emscripten activado primero:

```bash
source /path/to/emsdk/emsdk_env.sh
```

Luego compila usando uno de estos scripts de npm:

**Opción 1 (Recomendada)**: Usar el script interactivo:

```bash
npm run build:wasm
```

**Opción 2**: Usar Makefile directamente:

```bash
# Con raylib (si lo tienes instalado)
RAYLIB_PATH=/ruta/a/raylib npm run build:wasm:make

# O sin especificar (buscará automáticamente)
npm run build:wasm:make
```

**Ayuda**: Para ver información sobre los scripts disponibles:

```bash
npm run build:wasm:help
```

> **Nota**: El script `build:wasm` automáticamente coloca los archivos compilados en `public/cpp/`, así que no necesitas moverlos manualmente.

## 💻 Desarrollo

### Desarrollo Normal

Para iniciar el servidor de desarrollo:

```bash
npm run dev
```

El proyecto estará disponible en `http://localhost:5173`

**Nota importante**: El servidor de desarrollo de Vite está configurado con headers CORS especiales necesarios para WebAssembly.

### Desarrollo con Auto-recompilación de C++

Si quieres que los cambios en C++ se recompilen automáticamente:

```bash
npm run dev:watch
```

Este comando:

- ✅ Inicia el servidor de desarrollo de Vite
- ✅ Observa cambios en archivos `.cpp` y `.h` en `src/cpp/`
- ✅ Recompila automáticamente cuando detecta cambios
- ✅ Vite detecta los nuevos archivos y recarga la página

**Nota**: Necesitas tener Emscripten activado antes de ejecutar `dev:watch`.

### Desarrollo Manual (Sin Auto-recompilación)

Si prefieres compilar manualmente:

```bash
# Terminal 1: Servidor de desarrollo
npm run dev

# Terminal 2: Cuando hagas cambios en C++, recompila
npm run build:wasm
```

## 🏗️ Estructura del Proyecto

```
tennis-emulator/
├── src/
│   ├── cpp/                  # Código C++ con raylib
│   │   ├── main.cpp          # Código principal (dibuja un rectángulo)
│   │   ├── Makefile          # Makefile completo
│   │   └── Makefile.simple   # Makefile simplificado (recomendado)
│   ├── App.tsx               # Componente principal de React
│   ├── App.css               # Estilos del componente
│   ├── main.tsx              # Punto de entrada de React
│   └── index.css             # Estilos globales
├── public/
│   └── cpp/                  # Archivos WebAssembly compilados (generados)
│       ├── tennis_emulator.js
│       └── tennis_emulator.wasm
├── package.json
├── vite.config.ts
├── tsconfig.json
└── README.md
```

## 🔧 Scripts Disponibles

### Scripts Principales

- `npm run dev` - Inicia el servidor de desarrollo (verifica que WebAssembly esté compilado)
- `npm run dev:watch` - **Inicia dev + watcher para C++** (recompila automáticamente)
- `npm run build` - Compila el proyecto React para producción
- `npm run preview` - Previsualiza la build de producción

### Scripts de WebAssembly

- `npm run build:raylib` - **Compila raylib para WebAssembly** (solo necesario una vez, o si cambias raylib)
- `npm run build:wasm` - **Compila el código C++ a WebAssembly** (recomendado, compila raylib automáticamente si es necesario)
- `npm run build:wasm:quick` - Compila sin preguntas (usado por el watcher)
- `npm run build:wasm:make` - Compila usando Makefile directamente
- `npm run build:wasm:help` - Muestra ayuda sobre cómo compilar WebAssembly

### Scripts de Utilidad

- `npm run setup` - Instala dependencias de Node.js
- `npm run clean` - Limpia archivos generados (dist, .wasm, .js compilados)

### Ejemplos de Uso

```bash
# Compilar WebAssembly con raylib personalizado
RAYLIB_PATH=/home/usuario/raylib npm run build:wasm:make

# Ver ayuda sobre compilación
npm run build:wasm:help

# Desarrollo completo con auto-recompilación
npm run dev:watch

# Desarrollo manual
npm run build:wasm && npm run dev
```

## 📝 Notas Importantes

### Headers CORS

El proyecto configura headers especiales en `vite.config.ts` para permitir WebAssembly con SharedArrayBuffer:

- `Cross-Origin-Opener-Policy: same-origin`
- `Cross-Origin-Embedder-Policy: require-corp`

### Rutas de Archivos

Los archivos `.wasm` y `.js` generados por Emscripten deben estar en `public/cpp/` para que sean accesibles desde el navegador.

### Compilación de Raylib

Compilar raylib para WebAssembly puede ser complejo. Algunos recursos útiles:

- [Raylib Web Examples](https://github.com/raysan5/raylib/tree/master/examples/others)
- [Emscripten Documentation](https://emscripten.org/docs/getting_started/index.html)

Si encuentras problemas, considera usar una versión precompilada de raylib-web o buscar ejemplos específicos de raylib + Emscripten.

### Auto-recompilación de C++

El script `dev:watch` usa `chokidar-cli` para observar cambios en archivos C++. Cuando detecta un cambio:

1. Ejecuta `build:wasm:quick` automáticamente
2. Los archivos se compilan y se copian a `public/cpp/`
3. Vite detecta los cambios y recarga la página

**Importante**: Necesitas tener Emscripten activado antes de ejecutar `dev:watch`.

## 🐛 Troubleshooting

### Error: "emcc: command not found"

- Asegúrate de haber activado Emscripten: `source emsdk/emsdk_env.sh`
- Verifica que Emscripten esté en tu PATH

### Error: "Cannot find raylib"

- Verifica que raylib esté instalado y compilado para WebAssembly
- Revisa la ruta en el Makefile o pasa `RAYLIB_PATH` como variable

### Error: "SharedArrayBuffer is not defined"

- Asegúrate de que el servidor tenga los headers CORS correctos configurados
- Verifica `vite.config.ts`
- Algunos navegadores requieren HTTPS para SharedArrayBuffer

### Error: "Module not found: /cpp/tennis_emulator.js"

- Asegúrate de haber compilado el código C++ primero
- Verifica que los archivos estén en `public/cpp/`
- Revisa la consola del navegador para más detalles

### Los cambios en C++ no se reflejan

- Si usas `npm run dev`, necesitas recompilar manualmente: `npm run build:wasm`
- Si usas `npm run dev:watch`, verifica que el watcher esté funcionando (deberías ver mensajes en la consola)
- Asegúrate de tener Emscripten activado si usas `dev:watch`

## 🎮 Próximos Pasos

- [x] Configurar raylib-web correctamente
- [ ] Agregar más formas y animaciones
- [ ] Implementar física básica
- [ ] Agregar controles de usuario
- [ ] Crear un juego de tenis simple

## 📚 Recursos

- [Raylib Documentation](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Emscripten Documentation](https://emscripten.org/docs/getting_started/index.html)
- [WebAssembly](https://webassembly.org/)
- [React Documentation](https://react.dev/)
- [Vite Documentation](https://vite.dev/)

## 📄 Licencia

Este proyecto es de código abierto y está disponible bajo la licencia MIT.
