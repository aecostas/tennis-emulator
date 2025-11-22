# Makefile para compilar C++ con raylib a WebAssembly usando Emscripten

# Variables
EMCC = emcc
SRC_DIR = .
BUILD_DIR = ..
TARGET = tennis_emulator
RAYLIB_PATH = $(HOME)/raylib

# Flags de compilación
CFLAGS = -Wall -Wextra -std=c++17
EMFLAGS = -s WASM=1 \
          -s USE_GLFW=3 \
          -s FULL_ES2=1 \
          -s USE_WEBGL2=1 \
          -s ASYNCIFY \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
          -s EXPORTED_FUNCTIONS='["_main","_init","_update","_draw"]' \
          -s ALLOW_MEMORY_GROWTH=1 \
          -s INITIAL_MEMORY=67108864 \
          -O2 \
          --shell-file shell_minimal.html \
          --no-entry

# Si raylib está instalado localmente, usar esa ruta
# Si no, intentar usar raylib-web (versión para web)
RAYLIB_WEB = $(shell if [ -d "raylib-web" ]; then echo "raylib-web"; else echo ""; fi)

# Archivos fuente
SOURCES = main.cpp

# Objetivo principal
all: $(BUILD_DIR)/$(TARGET).js

# Compilar a WebAssembly
$(BUILD_DIR)/$(TARGET).js: $(SOURCES)
	@echo "Compilando con Emscripten..."
	@echo "NOTA: Asegúrate de tener Emscripten instalado y raylib-web configurado"
	@echo ""
	@echo "Si no tienes raylib-web, puedes instalarlo con:"
	@echo "  git clone https://github.com/raysan5/raylib.git raylib"
	@echo "  cd raylib/src"
	@echo "  emcc -c rcore.c -o rcore.o -Os -DPLATFORM_WEB"
	@echo ""
	@echo "O usar raylib-web precompilado desde:"
	@echo "  https://github.com/raysan5/raylib/tree/master/src"
	@echo ""
	$(EMCC) $(CFLAGS) $(SOURCES) $(EMFLAGS) -o $(BUILD_DIR)/$(TARGET).js \
		-I$(RAYLIB_PATH)/src \
		-L$(RAYLIB_PATH)/src \
		-lraylib \
		--preload-file assets@/assets || \
	$(EMCC) $(CFLAGS) $(SOURCES) $(EMFLAGS) -o $(BUILD_DIR)/$(TARGET).js \
		-DPLATFORM_WEB \
		-s USE_GLFW=3 \
		-s FULL_ES2=1 \
		-s USE_WEBGL2=1 || \
	echo "Error: Necesitas tener raylib instalado. Ver README.md para instrucciones."

# Limpiar archivos generados
clean:
	rm -f $(BUILD_DIR)/$(TARGET).js $(BUILD_DIR)/$(TARGET).wasm $(BUILD_DIR)/$(TARGET).html

.PHONY: all clean

