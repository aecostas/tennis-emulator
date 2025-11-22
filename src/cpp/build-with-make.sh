#!/bin/bash

# Script para compilar usando Makefile con búsqueda automática de raylib

set -e

# Buscar raylib si no está definido
if [ -z "$RAYLIB_PATH" ]; then
    RAYLIB_PATH=$(find ~ -type d -name "raylib" 2>/dev/null | head -1)
fi

if [ ! -z "$RAYLIB_PATH" ]; then
    echo "✅ Usando raylib en: $RAYLIB_PATH"
    export RAYLIB_PATH
else
    echo "⚠️  Raylib no encontrado. Compilando sin raylib..."
fi

# Ejecutar make
cd "$(dirname "$0")"
make -f Makefile.simple

