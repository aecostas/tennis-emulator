# Guía para Compilar Raylib para WebAssembly

Esta guía te ayudará a compilar raylib para WebAssembly usando Emscripten.

## Opción 1: Compilar Raylib desde el Código Fuente

### Paso 1: Clonar Raylib

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib/src
```

### Paso 2: Compilar Raylib para Web

Raylib necesita ser compilado con Emscripten. Aquí hay un enfoque básico:

```bash
# Asegúrate de tener Emscripten activado
source /path/to/emsdk/emsdk_env.sh

# Compilar los archivos fuente principales de raylib
# Nota: Esto es un ejemplo simplificado, puede requerir ajustes

emcc -c rcore.c -o rcore.o -Os -DPLATFORM_WEB -I. -I../src
emcc -c rshapes.c -o rshapes.o -Os -DPLATFORM_WEB -I. -I../src
emcc -c rtextures.c -o rtextures.o -Os -DPLATFORM_WEB -I. -I../src
emcc -c rtext.c -o rtext.o -Os -DPLATFORM_WEB -I. -I../src
emcc -c rmodels.c -o rmodels.o -Os -DPLATFORM_WEB -I. -I../src
emcc -c raudio.c -o raudio.o -Os -DPLATFORM_WEB -I. -I../src

# Crear librería estática
emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o raudio.o
```

### Paso 3: Usar la Librería Compilada

Una vez compilada, puedes usar la ruta a raylib en el Makefile:

```bash
cd /path/to/tennis-emulator/src/cpp
make -f Makefile.simple RAYLIB_PATH=/path/to/raylib
```

## Opción 2: Usar Raylib Precompilado (si está disponible)

Algunos proyectos proporcionan versiones precompiladas de raylib para WebAssembly. Busca en:

- [Raylib Releases](https://github.com/raysan5/raylib/releases)
- [Raylib Web Examples](https://github.com/raysan5/raylib/tree/master/examples/others)

## Opción 3: Compilar con el Makefile de Raylib (Avanzado)

Si raylib tiene un Makefile que soporta Emscripten:

```bash
cd raylib/src
# Revisa el Makefile para opciones de compilación web
make PLATFORM=PLATFORM_WEB
```

## Notas Importantes

1. **Dependencias**: Raylib para web requiere GLFW3, que Emscripten proporciona a través de `-s USE_GLFW=3`.

2. **WebGL**: Raylib usa WebGL para renderizado. Asegúrate de que los flags incluyan `-s USE_WEBGL2=1` o `-s FULL_ES2=1`.

3. **Asyncify**: Puede ser necesario para algunas funciones de raylib que son síncronas pero necesitan ser asíncronas en web.

4. **Headers**: Asegúrate de incluir los headers correctos con `-I/path/to/raylib/src`.

## Verificación

Para verificar que raylib está correctamente compilado:

```bash
# Verificar que los archivos .o y .a existen
ls -la /path/to/raylib/src/*.o
ls -la /path/to/raylib/src/libraylib.a
```

## Troubleshooting

### Error: "undefined reference to raylib functions"

- Verifica que la librería esté correctamente enlazada
- Asegúrate de que todos los archivos fuente necesarios estén compilados

### Error: "PLATFORM_WEB not defined"

- Agrega `-DPLATFORM_WEB` a los flags de compilación

### Error: "GLFW functions not found"

- Asegúrate de usar `-s USE_GLFW=3` en los flags de Emscripten

## Recursos Adicionales

- [Raylib Web Platform](https://github.com/raysan5/raylib/wiki/Working-on-Web)
- [Emscripten Porting Guide](https://emscripten.org/docs/porting/index.html)
- [Raylib Examples for Web](https://github.com/raysan5/raylib/tree/master/examples/others)
