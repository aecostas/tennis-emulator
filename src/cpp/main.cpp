#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "Rect.h"
#include "Ball.h"
#include <cstdlib>
#include <ctime>
#include <vector>

// Dimensiones de la ventana
const int screenWidth = 800;
const int screenHeight = 600;

// Constantes para las pelotas
const int NUM_BALLS = 10;
const float BALL_RADIUS = 15.0f;
const float BALL_SPACING = 5.0f; // Espacio entre pelotas
const float INITIAL_HEIGHT = 50.0f;
const float MIN_RESTITUTION = 0.7f;
const float MAX_RESTITUTION = 0.9f;

// Array de pelotas
std::vector<Ball> balls;

// Función de actualización (game loop)
void UpdateDrawFrame(void);

int main(void)
{
    // Inicializar semilla para números aleatorios
    srand(time(nullptr));
    
    // Inicializar raylib
    InitWindow(screenWidth, screenHeight, "Tennis Emulator - Raylib + WebAssembly");
    
    SetTargetFPS(60);
    
    // Crear 20 pelotas con restitución aleatoria entre 0.7 y 0.9
    float startX = 50.0f; // Posición inicial X
    Color colors[] = {RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE, LIME, MAROON};
    
    for (int i = 0; i < NUM_BALLS; i++) {
        // Generar restitución aleatoria entre 0.7 y 0.9
        float restitution = MIN_RESTITUTION + ((float)rand() / (float)RAND_MAX) * (MAX_RESTITUTION - MIN_RESTITUTION);
        
        // Calcular posición X (una al lado de la otra)
        float x = startX + i * (BALL_RADIUS * 2 + BALL_SPACING);
        
        // Color alternado
        Color ballColor = colors[i % 10];
        
        // Crear pelota
        balls.push_back(Ball(x, BALL_RADIUS, INITIAL_HEIGHT, ballColor, 0.0f, restitution));
    }
    
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
    float deltaTime = GetFrameTime();
    
    // Actualizar todas las pelotas
    for (auto& ball : balls) {
        ball.Update(deltaTime, screenHeight);
    }
    
    // Dibujado
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    // Dibujar todas las pelotas
    for (auto& ball : balls) {
        ball.Draw();
    }
    
    // Dibujar texto informativo
    DrawText("20 Pelotas con diferentes restituciones", 10, 10, 20, DARKGRAY);
    DrawText("Restitucion: 0.7 - 0.9 (aleatorio)", 10, 35, 20, DARKGRAY);
    
    EndDrawing();
}

