#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "Ball3d.h"
#include "Court.h"
#include <cstdlib>
#include <ctime>

// Dimensiones de la ventana
const int screenWidth = 800;
const int screenHeight = 600;

// Pista de tenis
const float COURT_WIDTH = 800.0f;
Court court(COURT_WIDTH);

// Cámara 3D
Camera camera;

// Pelota - empieza cerca de la cámara (extremo inferior de la pista) y se mueve hacia adelante
Ball3D pelota({court.GetMaxX() / 2, 50.0f, 50.0f}, 15.0f, RED, {0.0f, -300.0f, 800.0f}, {20.0f, 0.0f, -10.0f});

// Función de actualización (game loop)
void UpdateDrawFrame(void);

int main(void)
{
    // Inicializar semilla
    srand(time(nullptr));

    // Inicializar ventana
    InitWindow(screenWidth, screenHeight, "Simulador de Tenis 3D");
    SetTargetFPS(60);

    // Configurar cámara 3D - en uno de los extremos de la pista con mejor ángulo para ver profundidad
    camera.position = {court.GetMaxX() / 2, 300.0f, -300.0f};  // posición más baja y más atrás para mejor perspectiva
    camera.target = {court.GetMaxX() / 2, 50.0f, court.GetMaxZ() * 0.6f};  // mira hacia adelante en la pista
    camera.up = {0.0f, 1.0f, 0.0f};                  // vector up estándar (Y positivo = arriba)
    camera.fovy = 70.0f;                              // campo de visión más amplio para ver mejor la profundidad
    camera.projection = CAMERA_PERSPECTIVE;

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void)
{
    float deltaTime = GetFrameTime();

    // Actualizar la pelota
    pelota.Update(deltaTime, court.GetFloorY(), court.GetMaxX(), court.GetMaxZ());

    // Dibujado
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Dibujar la pista (superficie + líneas)
    court.Draw();

    // Dibujar la pelota
    pelota.Draw();

    EndMode3D();

    // Texto informativo
    DrawText("Pelota de tenis 3D con rebote y spin", 10, 10, 20, DARKGRAY);

    EndDrawing();
}
