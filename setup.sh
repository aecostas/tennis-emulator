#!/bin/bash

# Script de configuración para Tennis Emulator
# Este script ayuda a configurar el entorno de desarrollo

set -e

echo "🚀 Configurando Tennis Emulator..."
echo ""

# Verificar si Emscripten está instalado
if ! command -v emcc &> /dev/null; then
    echo "❌ Emscripten no está instalado o no está en el PATH"
    echo ""
    echo "Para instalar Emscripten:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    echo ""
    exit 1
fi

echo "✅ Emscripten encontrado: $(emcc --version | head -n 1)"
echo ""

# Verificar si raylib está disponible
RAYLIB_PATH=$(find ~ -type d -name "raylib" 2>/dev/null | head -1)

if [ -z "$RAYLIB_PATH" ]; then
    echo "⚠️  Raylib no encontrado automáticamente"
    echo ""
    echo "Para instalar raylib:"
    echo "  git clone https://github.com/raysan5/raylib.git"
    echo "  cd raylib/src"
    echo ""
    echo "Luego compila raylib para web (esto requiere Emscripten activado)"
    echo ""
    read -p "¿Tienes raylib instalado? Ingresa la ruta (o presiona Enter para continuar sin raylib): " user_path
    
    if [ ! -z "$user_path" ]; then
        RAYLIB_PATH="$user_path"
    fi
else
    echo "✅ Raylib encontrado en: $RAYLIB_PATH"
fi

# Instalar dependencias de Node.js
echo ""
echo "📦 Instalando dependencias de Node.js..."
npm install

# Crear directorio para archivos compilados
echo ""
echo "📁 Creando directorios necesarios..."
mkdir -p public/cpp

echo ""
echo "✅ Configuración completada!"
echo ""
echo "Próximos pasos:"
echo "  1. Si tienes raylib, compila el código C++:"
echo "     cd src/cpp"
if [ ! -z "$RAYLIB_PATH" ]; then
    echo "     make -f Makefile.simple RAYLIB_PATH=$RAYLIB_PATH"
else
    echo "     make -f Makefile.simple RAYLIB_PATH=/ruta/a/raylib"
fi
echo ""
echo "  2. Mueve los archivos compilados a public/cpp/"
echo "     cp src/cpp/tennis_emulator.* public/cpp/"
echo ""
echo "  3. Inicia el servidor de desarrollo:"
echo "     npm run dev"
echo ""

