#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "Rect.h"

// Dimensiones de la ventana
const int screenWidth = 800;
const int screenHeight = 600;

// Instancia del rectángulo
Rect box(0, 100, 200, 150, RED, 2.0f);
Rect box2(0, 100, 200, 150, BLUE, 2.0f);

// Función de actualización (game loop)
void UpdateDrawFrame(void);

int main(void)
{
    // Inicializar raylib
    InitWindow(screenWidth, screenHeight, "Tennis Emulator - Raylib + WebAssembly");
    
    SetTargetFPS(60);
    
#ifdef PLATFORM_WEB
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    // Loop principal para plataformas nativas
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif
    
    CloseWindow();
    return 0;
}

// Función de actualización y dibujado
void UpdateDrawFrame(void)
{
    // Actualizar el rectángulo
    box.Update(screenWidth);
    
    // Dibujado
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    // Dibujar el rectángulo
    box.Draw();
    box2.Draw();
    
    // Dibujar texto informativo
    DrawText("Rectangulo dibujado con C++ y Raylib", 10, 10, 20, DARKGRAY);
    DrawText("Compilado a WebAssembly", 10, 35, 20, DARKGRAY);
    
    EndDrawing();
}

