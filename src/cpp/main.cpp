#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
    #include <cstdio>
    #include <cstdarg>
    #include <string>
    
    // Función helper para hacer console.log desde C++ (soporta formato como printf)
    inline void console_log(const char* format, ...) {
        char buffer[512];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        std::string script = "console.log('";
        script += buffer;
        script += "');";
        emscripten_run_script(script.c_str());
    }
#else
    // Para plataformas no-web, usar printf normal
    #include <cstdio>
    #include <cstdarg>
    inline void console_log(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }
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
const float ZOOM_SENSITIVITY = 20.0f;
const float PAN_SENSITIVITY = 0.5f;
const float MIN_DISTANCE = 100.0f;
const float MAX_DISTANCE = 2000.0f;

// Pelota - se inicializará en main()
Vector3 ballInitialPos;
float ballInitialSpeed = 1500.0f;  // Velocidad inicial (magnitud)
float ballInitialAngle = 0.0f;     // Ángulo horizontal (en grados, 0 = hacia adelante)
float ballInitialElevation = -20.0f; // Ángulo vertical (en grados, negativo = hacia abajo)
Vector3 ballInitialSpin = {20.0f, 0.0f, -10.0f};
Ball3D pelota({0.0f, 50.0f, 50.0f}, 15.0f, RED, {0.0f, 0.0f, 0.0f}); // Empieza sin movimiento

// Funciones
void UpdateDrawFrame(void);
void UpdateCameraControls(void);
Vector3 CalculateCameraPosition(Vector3 target, float distance, float angleX, float angleY);

// Función auxiliar para calcular velocidad desde ángulo y velocidad
Vector3 CalculateVelocityFromAngle(float speed, float angleDeg, float elevationDeg) {
    // Convertir grados a radianes
    float angleRad = angleDeg * M_PI / 180.0f;
    float elevationRad = elevationDeg * M_PI / 180.0f;
    
    // Calcular componentes de velocidad
    float velX = speed * cosf(elevationRad) * sinf(angleRad);
    float velY = speed * sinf(elevationRad);
    float velZ = speed * cosf(elevationRad) * cosf(angleRad);
    
    return {velX, velY, velZ};
}

// Función exportada para disparar la pelota desde JavaScript
extern "C" {
    void EMSCRIPTEN_KEEPALIVE shootBall() {
        Vector3 vel = CalculateVelocityFromAngle(ballInitialSpeed, ballInitialAngle, ballInitialElevation);
        pelota.Reset(ballInitialPos, vel, ballInitialSpin);
    }
    
    // Función para configurar el ángulo y velocidad inicial
    void EMSCRIPTEN_KEEPALIVE setBallAngle(float angleDeg, float elevationDeg, float speed) {
        ballInitialAngle = angleDeg;
        ballInitialElevation = elevationDeg;
        ballInitialSpeed = speed;
    }
}


int main(void)
{
    // Inicializar semilla
    srand(time(nullptr));

    // Habilitar anti-aliasing para suavizar las líneas
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Inicializar ventana
    InitWindow(screenWidth, screenHeight, "Simulador de Tenis 3D");
    
    // Verificar que la ventana se inicializó correctamente
    if (!IsWindowReady()) {
        #ifdef PLATFORM_WEB
        emscripten_run_script("console.error('[WASM] Error: Ventana no inicializada');");
        #endif
        return 1;
    }
    
    SetTargetFPS(60);
    
    // Ejemplo de uso de console_log
    console_log("Simulador de Tenis 3D inicializado");
    console_log("Ancho de pista: %.2f unidades", COURT_WIDTH);

    // Inicializar posición inicial de la pelota
    ballInitialPos = {court.GetMaxX() / 2, 50.0f, 50.0f};
    pelota.Reset(ballInitialPos, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
    
    // Calcular velocidad inicial basada en ángulos
    Vector3 initialVel = CalculateVelocityFromAngle(ballInitialSpeed, ballInitialAngle, ballInitialElevation);

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
            
            // Limitar el ángulo vertical para evitar voltear la cámara y ver desde abajo
            if (cameraAngleY > 1.5f) cameraAngleY = 1.5f;
            if (cameraAngleY < -0.1f) cameraAngleY = -0.1f;  // No permitir ver desde abajo de la pista
            
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

    
    // Verificar que la ventana esté lista
    if (!IsWindowReady()) {
        return;
    }
    
    float deltaTime = GetFrameTime();

    // Actualizar controles de cámara
    UpdateCameraControls();

    // Actualizar la pelota (solo si está en movimiento)
    float netZ = court.GetMaxZ() / 2.0f;  // Centro de la pista (donde está la red)
    pelota.Update(deltaTime, court.GetFloorY(), court.GetMaxX(), court.GetMaxZ(), netZ, court);

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
    DrawText("Pelota de tenis 3D con rebote y spin!!!", 10, 10, 20, DARKGRAY);
    DrawText("Click izquierdo + arrastrar: Rotar | Rueda: Zoom | Shift + arrastrar: Pan", 10, 35, 16, DARKGRAY);

    EndDrawing();
}
