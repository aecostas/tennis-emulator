#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "Ball3d.h"
#include "Court.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// Dimensiones de la ventana
const int screenWidth = 800;
const int screenHeight = 600;

// Pista de tenis
const float COURT_WIDTH = 800.0f;
Court court(COURT_WIDTH);

// Cámara 3D
Camera camera;

// Variables para control de cámara con ratón
Vector2 lastMousePos = {0.0f, 0.0f};
bool isMouseDragging = false;
float cameraDistance = 0.0f;
float cameraAngleX = 0.0f;  // Ángulo horizontal (azimuth)
float cameraAngleY = 0.0f;  // Ángulo vertical (elevation)
const float ROTATION_SENSITIVITY = 0.005f;
const float ZOOM_SENSITIVITY = 5.0f;
const float PAN_SENSITIVITY = 0.5f;
const float MIN_DISTANCE = 100.0f;
const float MAX_DISTANCE = 2000.0f;

// Pelota - empieza cerca de la cámara (extremo inferior de la pista) y se mueve hacia adelante
Ball3D pelota({court.GetMaxX() / 2, 50.0f, 50.0f}, 15.0f, RED, {0.0f, -300.0f, 800.0f}, {20.0f, 0.0f, -10.0f});

// Funciones
void UpdateDrawFrame(void);
void UpdateCameraControls(void);
Vector3 CalculateCameraPosition(Vector3 target, float distance, float angleX, float angleY);

int main(void)
{
    // Inicializar semilla
    srand(time(nullptr));

    // Inicializar ventana
    InitWindow(screenWidth, screenHeight, "Simulador de Tenis 3D");
    SetTargetFPS(60);

    // Configurar cámara 3D - en uno de los extremos de la pista con mejor ángulo para ver profundidad
    camera.target = {court.GetMaxX() / 2, 50.0f, court.GetMaxZ() * 0.6f};  // punto de enfoque
    cameraDistance = 500.0f;  // Distancia inicial
    cameraAngleX = 0.0f;       // Ángulo horizontal inicial
    cameraAngleY = 0.5f;       // Ángulo vertical inicial (ligeramente hacia abajo)
    camera.position = CalculateCameraPosition(camera.target, cameraDistance, cameraAngleX, cameraAngleY);
    camera.up = {0.0f, 1.0f, 0.0f};                  // vector up estándar (Y positivo = arriba)
    camera.fovy = 70.0f;                              // campo de visión más amplio para ver mejor la profundidad
    camera.projection = CAMERA_PERSPECTIVE;
    
    lastMousePos = GetMousePosition();

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

Vector3 CalculateCameraPosition(Vector3 target, float distance, float angleX, float angleY) {
    // Calcular posición de la cámara usando coordenadas esféricas
    float cosY = cosf(angleY);
    float x = target.x + distance * cosY * sinf(angleX);
    float y = target.y + distance * sinf(angleY);
    float z = target.z + distance * cosY * cosf(angleX);
    return {x, y, z};
}

void UpdateCameraControls(void) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mouseDelta = {mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y};
    
    // Shift + arrastrar: pan (mover el target) - tiene prioridad sobre la rotación
    bool isShiftPressed = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
    
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (!isMouseDragging) {
            isMouseDragging = true;
        } else if (isShiftPressed) {
            // Pan: mover el target (y la cámara se mueve con él)
            // Calcular vectores derecho y arriba basados en la orientación de la cámara
            Vector3 forward = {camera.target.x - camera.position.x, 
                              camera.target.y - camera.position.y, 
                              camera.target.z - camera.position.z};
            float forwardLength = sqrtf(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
            if (forwardLength > 0.001f) {
                forward.x /= forwardLength;
                forward.y /= forwardLength;
                forward.z /= forwardLength;
            }
            
            // Vector right: perpendicular a forward y up
            Vector3 worldUp = {0.0f, 1.0f, 0.0f};
            Vector3 right = {
                forward.z * worldUp.y - forward.y * worldUp.z,
                forward.x * worldUp.z - forward.z * worldUp.x,
                forward.y * worldUp.x - forward.x * worldUp.y
            };
            float rightLength = sqrtf(right.x * right.x + right.y * right.y + right.z * right.z);
            if (rightLength > 0.001f) {
                right.x /= rightLength;
                right.y /= rightLength;
                right.z /= rightLength;
            }
            
            // Vector up: perpendicular a forward y right
            Vector3 up = {
                right.y * forward.z - right.z * forward.y,
                right.z * forward.x - right.x * forward.z,
                right.x * forward.y - right.y * forward.x
            };
            
            // Mover el target según el movimiento del ratón
            float panSpeed = PAN_SENSITIVITY * (cameraDistance / 500.0f); // Ajustar velocidad según distancia
            camera.target.x += (right.x * mouseDelta.x - up.x * mouseDelta.y) * panSpeed;
            camera.target.y += (right.y * mouseDelta.x - up.y * mouseDelta.y) * panSpeed;
            camera.target.z += (right.z * mouseDelta.x - up.z * mouseDelta.y) * panSpeed;
            
            // Actualizar posición de la cámara para mantener la distancia
            camera.position = CalculateCameraPosition(camera.target, cameraDistance, cameraAngleX, cameraAngleY);
        } else {
            // Rotar cámara alrededor del target
            cameraAngleX -= mouseDelta.x * ROTATION_SENSITIVITY;
            cameraAngleY += mouseDelta.y * ROTATION_SENSITIVITY;
            
            // Limitar el ángulo vertical para evitar voltear la cámara
            if (cameraAngleY > 1.5f) cameraAngleY = 1.5f;
            if (cameraAngleY < -1.5f) cameraAngleY = -1.5f;
            
            camera.position = CalculateCameraPosition(camera.target, cameraDistance, cameraAngleX, cameraAngleY);
        }
    } else {
        isMouseDragging = false;
    }
    
    // Rueda del ratón: zoom (cambiar distancia)
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0.0f) {
        cameraDistance -= wheelMove * ZOOM_SENSITIVITY;
        if (cameraDistance < MIN_DISTANCE) cameraDistance = MIN_DISTANCE;
        if (cameraDistance > MAX_DISTANCE) cameraDistance = MAX_DISTANCE;
        camera.position = CalculateCameraPosition(camera.target, cameraDistance, cameraAngleX, cameraAngleY);
    }
    
    lastMousePos = mousePos;
}

void UpdateDrawFrame(void)
{
    float deltaTime = GetFrameTime();

    // Actualizar controles de cámara
    UpdateCameraControls();

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
    DrawText("Click izquierdo + arrastrar: Rotar | Rueda: Zoom | Shift + arrastrar: Pan", 10, 35, 16, DARKGRAY);

    EndDrawing();
}
