#!/bin/bash

# Script de compilación simplificado para Tennis Emulator
# Este script compila el código C++ a WebAssembly

set -e
set -o pipefail

echo "🔨 Compilando Tennis Emulator a WebAssembly..."
echo ""

# Intentar activar Emscripten automáticamente si no está en el PATH
if ! command -v emcc &> /dev/null; then
    echo "⚠️  Emscripten no está en el PATH, intentando activarlo automáticamente..."
    
    # Buscar emsdk_env.sh en ubicaciones comunes
    EMSDK_ENV=""
    if [ -f "$HOME/emsdk/emsdk_env.sh" ]; then
        EMSDK_ENV="$HOME/emsdk/emsdk_env.sh"
    elif [ -f "$HOME/dev/personal/emsdk/emsdk_env.sh" ]; then
        EMSDK_ENV="$HOME/dev/personal/emsdk/emsdk_env.sh"
    else
        EMSDK_ENV=$(find ~ -name "emsdk_env.sh" -type f 2>/dev/null | head -1)
    fi
    
    if [ ! -z "$EMSDK_ENV" ] && [ -f "$EMSDK_ENV" ]; then
        echo "   Activando Emscripten desde: $EMSDK_ENV"
        source "$EMSDK_ENV" > /dev/null 2>&1
        
        # Verificar que ahora esté disponible
        if command -v emcc &> /dev/null; then
            echo "   ✅ Emscripten activado correctamente"
        else
            echo "   ❌ No se pudo activar Emscripten automáticamente"
            echo "   Activa Emscripten manualmente con: source $EMSDK_ENV"
            exit 1
        fi
    else
        echo "   ❌ No se encontró emsdk_env.sh"
        echo "   Activa Emscripten manualmente con: source /path/to/emsdk/emsdk_env.sh"
        exit 1
    fi
fi

# Buscar raylib
RAYLIB_PATH=${RAYLIB_PATH:-$(find ~ -type d -name "raylib" 2>/dev/null | head -1)}

if [ -z "$RAYLIB_PATH" ]; then
    echo "⚠️  Raylib no encontrado automáticamente"
    echo ""
    echo "Para compilar sin raylib (solo con funciones básicas de WebGL),"
    echo "este script intentará compilar con flags mínimos."
    echo ""
    read -p "¿Tienes raylib instalado? Ingresa la ruta (o presiona Enter para continuar sin raylib): " user_path
    
    if [ ! -z "$user_path" ] && [ -d "$user_path" ]; then
        RAYLIB_PATH="$user_path"
    fi
fi

# Directorios
SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SRC_DIR/../../public/cpp"
TARGET="tennis_emulator"

# Crear directorio de salida
mkdir -p "$BUILD_DIR"

echo "📂 Directorio fuente: $SRC_DIR"
echo "📂 Directorio de salida: $BUILD_DIR"
echo ""

# Flags de compilación básicos
FLAGS=(
    -s WASM=1
    -s USE_GLFW=3
    -s USE_WEBGL2=1
    -s GL_PREINITIALIZED_CONTEXT=0
    -s ASYNCIFY
    -s ALLOW_MEMORY_GROWTH=1
    -s INITIAL_MEMORY=67108864
    -s MODULARIZE=1
    -s EXPORT_NAME="createTennisEmulatorModule"
    -s USE_GLFW=3
    -s USE_WEBGL2=1
    -s FULL_ES3=1
    -s MIN_WEBGL_VERSION=2
    -s MAX_WEBGL_VERSION=2
    -O2
    -DPLATFORM_WEB
)

# Si tenemos raylib, verificar si está compilado y compilarlo si es necesario
if [ ! -z "$RAYLIB_PATH" ] && [ -d "$RAYLIB_PATH/src" ]; then
    RAYLIB_LIB="$RAYLIB_PATH/src/libraylib.a"
    
    # Verificar si raylib está compilado PARA WEBASSEMBLY
    RAYLIB_NEEDS_RECOMPILE=false
    if [ ! -f "$RAYLIB_LIB" ]; then
        RAYLIB_NEEDS_RECOMPILE=true
        echo "⚠️  Raylib encontrado pero no está compilado para WebAssembly"
    else
        # Verificar si fue compilado para WebAssembly
        if [ -f "$RAYLIB_PATH/src/rcore.o" ]; then
            if ! strings "$RAYLIB_PATH/src/rcore.o" 2>/dev/null | grep -q "emscripten\|__wasm" && ! file "$RAYLIB_PATH/src/rcore.o" 2>/dev/null | grep -q "WebAssembly\|wasm"; then
                RAYLIB_NEEDS_RECOMPILE=true
                echo "⚠️  Raylib está compilado pero NO para WebAssembly"
            fi
        fi
    fi
    
    if [ "$RAYLIB_NEEDS_RECOMPILE" = true ]; then
        echo ""
        read -p "¿Deseas compilar raylib para WebAssembly ahora? (s/n): " compile_raylib
        
        if [ "$compile_raylib" = "s" ] || [ "$compile_raylib" = "S" ] || [ "$compile_raylib" = "y" ] || [ "$compile_raylib" = "Y" ]; then
            echo ""
            echo "🔨 Compilando raylib para WebAssembly..."
            "$(dirname "$0")/compile-raylib.sh" || {
                echo "❌ Error compilando raylib. Continuando sin raylib..."
                RAYLIB_PATH=""
            }
        else
            echo "⚠️  Continuando sin raylib (funcionalidad limitada)"
            RAYLIB_PATH=""
        fi
    fi
    
    # Si tenemos raylib (compilado o ya estaba compilado), agregar flags
    if [ ! -z "$RAYLIB_PATH" ] && [ -f "$RAYLIB_LIB" ]; then
        echo "✅ Usando raylib en: $RAYLIB_PATH"
        echo "   Librería: $RAYLIB_LIB"
        FLAGS+=(-I"$RAYLIB_PATH/src")
        FLAGS+=(-L"$RAYLIB_PATH/src")
        FLAGS+=(-lraylib)
        # También necesitamos los headers de raylib
        if [ -f "$RAYLIB_PATH/src/raylib.h" ]; then
            echo "   Headers encontrados"
        else
            echo "   ⚠️  Advertencia: raylib.h no encontrado"
        fi
    else
        echo "⚠️  Raylib no disponible - el código se compilará pero no funcionará correctamente"
    fi
else
    echo "⚠️  Raylib no encontrado"
    echo "   El código intentará compilar sin raylib (puede fallar)"
    echo "   Para instalar raylib: git clone https://github.com/raysan5/raylib.git"
fi

# Compilar
echo ""
echo "🔨 Compilando..."
echo "   Flags: ${FLAGS[*]}"
cd "$SRC_DIR"

# Compilar y capturar el código de salida correctamente
if emcc main.cpp Court.cpp "${FLAGS[@]}" -o "$BUILD_DIR/$TARGET.js" 2>&1 | tee /tmp/emcc_output.log; then
    echo ""
    echo "✅ Compilación exitosa!"
    echo "   Archivos generados en: $BUILD_DIR"
    echo "   - $TARGET.js"
    echo "   - $TARGET.wasm"
    echo ""
    echo "🚀 Ahora puedes ejecutar: npm run dev"
else
    echo ""
    echo "❌ Error durante la compilación. Últimas líneas del log:"
    tail -20 /tmp/emcc_output.log
    echo ""
    echo "❌ Error en la compilación"
    echo "   Verifica que Emscripten esté correctamente instalado"
    echo "   y que raylib esté disponible si es necesario"
    exit 1
fi

