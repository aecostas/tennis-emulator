#ifndef BALL3D_H
#define BALL3D_H

#include "raylib.h"
#include <cmath>

class Ball3D {
private:
    Vector3 position;       // Posición 3D
    float radius;
    Color color;

    Vector3 velocity;       // Velocidad 3D
    bool isMoving;

    const float gravity = 980.0f;      // Gravedad en px/s^2
    const float restitution = 0.7f;    // Rebote vertical
    const float minVelocity = 20.0f;   // Umbral para detener rebote
    const float frictionXZ = 0.98f;    // Reducción de velocidad horizontal al rebotar
    Vector3 spin;                       // Efecto de spin (X,Z) en px/s

public:
    Ball3D(Vector3 pos, float rad, Color col, Vector3 vel, Vector3 spn = {0.0f, 0.0f, 0.0f})
        : position(pos), radius(rad), color(col), velocity(vel), spin(spn), isMoving(true) {}

        void Update(float deltaTime, float floorY, float maxX, float maxZ) {
            if (!isMoving) return;
        
            // Aplicar gravedad vertical (hacia abajo)
            velocity.y -= gravity * deltaTime;
        
            // Actualizar posición
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
            position.z += velocity.z * deltaTime;
        
            // Rebote con el suelo
            if (position.y <= floorY + radius) {
                position.y = floorY + radius;
                velocity.y = -velocity.y * restitution;
        
                // Aplicar spin lateral y fricción horizontal
                velocity.x = velocity.x * frictionXZ + spin.x;
                velocity.z = velocity.z * frictionXZ + spin.z;
        
                // Parar la pelota si el rebote vertical es demasiado pequeño
                if (std::abs(velocity.y) < minVelocity) {
                    velocity = {0.0f, 0.0f, 0.0f};
                    isMoving = false;
                }
            }
        
            // Parar si sale de los límites de la pista
            if (position.x < 0 || position.x > maxX || position.z < 0 || position.z > maxZ) {
                isMoving = false;
            }
        }
        

    void Draw() {
        DrawSphere(position, radius, color);
        DrawSphereWires(position, radius, 16, 16, BLACK);
    }

    // Getters
    bool GetIsMoving() const { return isMoving; }
    Vector3 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
};
#endif // BALL3D_H
