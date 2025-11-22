#!/bin/bash

set -e

echo "🔍 Buscando raylib…"

# Localizar raylib automáticamente si no está definido
RAYLIB_PATH=${RAYLIB_PATH:-$(find ~ -type d -name "raylib" 2>/dev/null | head -1)}

if [ -z "$RAYLIB_PATH" ]; then
    echo "❌ No se encontró raylib"
    echo "Clona raylib y define RAYLIB_PATH:"
    echo "  git clone https://github.com/raysan5/raylib.git"
    echo "  export RAYLIB_PATH=\$(pwd)/raylib"
    exit 1
fi

echo "📁 Raylib encontrado en: $RAYLIB_PATH"

RAYLIB_SRC="$RAYLIB_PATH/src"

cd "$RAYLIB_SRC"

echo "🔨 Compilando raylib para WebAssembly (WebGL2)…"
echo ""

FILES=(
    rcore.c
    rshapes.c
    rtextures.c
    rtext.c
    rmodels.c
    raudio.c
    utils.c
)

# Flags de compilación correctas
COMMON_FLAGS=(
    -Os
    -DPLATFORM_WEB
    -DGLFW_WASM
    -D__EMSCRIPTEN__
    -DGRAPHICS_API_OPENGL_ES3
    -I.
    -I..
)

# Compilar cada archivo .c
for f in "${FILES[@]}"; do
    echo "  → Compilando $f"
    emcc -c "$f" -o "${f%.c}.o" "${COMMON_FLAGS[@]}"
done

# Crear librería estática
echo ""
echo "📦 Empaquetando librería libraylib.a"
emar rcs libraylib.a *.o

# Mover librería a public/cpp/
TARGET_LIB="../../public/cpp/libraylib.a"
echo ""
echo "📁 Copiando librería a: $TARGET_LIB"
mkdir -p ../../public/cpp/
cp libraylib.a "$TARGET_LIB"

# Limpiar objetos
rm -f *.o

echo ""
echo "✅ Raylib WebAssembly compilado con éxito!"
echo "   Librería disponible en: public/cpp/libraylib.a"
