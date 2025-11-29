#ifndef BALL3D_H
#define BALL3D_H

#include "raylib.h"
#include "Court.h"
#include <cmath>

class Ball3D {
private:
    Vector3 position;       // Posición 3D
    float radius;
    Color color;

    Vector3 velocity;       // Velocidad 3D
    bool isMoving;
    float previousZ;        // Posición Z anterior para detectar cruce de la red

    const float gravity = 980.0f;      // Gravedad en px/s^2
    const float restitution = 0.7f;    // Rebote vertical
    const float minVelocity = 20.0f;   // Umbral para detener rebote
    const float frictionXZ = 0.98f;    // Reducción de velocidad horizontal al rebotar
    Vector3 spin;                       // Efecto de spin (X,Z) en px/s

    // Función para detectar y manejar colisión con la red
    void CheckNetCollision(Vector3& newPosition, float netZ, float floorY, const Court& court) {
        // Determinar la dirección del movimiento en Z
        float deltaZ = newPosition.z - position.z;
        if (std::abs(deltaZ) <= 0.001f) {  // No hay movimiento significativo en Z
            return;
        }
        
        // Calcular el borde de la pelota que está más cerca de la red
        // Si se mueve hacia adelante (deltaZ > 0), el borde delantero es position.z + radius
        // Si se mueve hacia atrás (deltaZ < 0), el borde trasero es position.z - radius
        float previousEdgeZ = position.z + (deltaZ > 0 ? radius : -radius);
        float newEdgeZ = newPosition.z + (deltaZ > 0 ? radius : -radius);
        
        // Verificar si el borde de la pelota está cruzando o cruzó la red
        bool previousWasBeforeNet = previousEdgeZ < netZ;
        bool newIsAfterNet = newEdgeZ >= netZ;
        bool previousWasAfterNet = previousEdgeZ > netZ;
        bool newIsBeforeNet = newEdgeZ < netZ;
        
        // Si el borde de la pelota cruzó la red
        if (!((previousWasBeforeNet && newIsAfterNet) || (previousWasAfterNet && newIsBeforeNet))) {
            return;
        }
        
        // Verificar si la altura de la pelota es menor que la altura de la red
        // Usar la posición intermedia (en la red) para la verificación
        float t = (netZ - previousEdgeZ) / (newEdgeZ - previousEdgeZ);
        float collisionX = position.x + (newPosition.x - position.x) * t;
        float collisionY = position.y + (newPosition.y - position.y) * t;
        
        float netHeight = court.GetNetHeightAtX(collisionX);
        float ballHeightAboveFloor = collisionY - floorY;
        
        // Si cualquier parte de la pelota está por debajo de la altura de la red
        // (el punto más bajo de la pelota es ballHeightAboveFloor - radius)
        if (ballHeightAboveFloor - radius >= netHeight) {
            return;
        }
        
        // Hay colisión: reposicionar la pelota del lado correcto de la red
        // Colocar el borde exterior de la pelota justo antes/después de la red
        if (previousWasBeforeNet) {
            // Venía desde antes de la red, dejarla justo antes
            newPosition.z = netZ - radius - 0.1f; // Pequeño margen para evitar que quede exactamente en la red
        } else {
            // Venía desde después de la red, dejarla justo después
            newPosition.z = netZ + radius + 0.1f; // Pequeño margen
        }
        
        // Detener el movimiento horizontal y dejar que caiga por gravedad
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        spin = {0.0f, 0.0f, 0.0f};
    }

public:
    Ball3D(Vector3 pos, float rad, Color col, Vector3 vel, Vector3 spn = {0.0f, 0.0f, 0.0f})
        : position(pos), radius(rad), color(col), velocity(vel), spin(spn), isMoving(true), previousZ(pos.z) {}

        void Update(float deltaTime, float floorY, float maxX, float maxZ, float netZ, const Court& court) {
            if (!isMoving) return;
        
            // Aplicar gravedad vertical (hacia abajo)
            velocity.y -= gravity * deltaTime;
        
            // Calcular nueva posición
            Vector3 newPosition = position;
            newPosition.x += velocity.x * deltaTime;
            newPosition.y += velocity.y * deltaTime;
            newPosition.z += velocity.z * deltaTime;
        
            // Detectar colisión con la red ANTES de actualizar la posición
            CheckNetCollision(newPosition, netZ, floorY, court);
            
            // Actualizar posición
            position = newPosition;
            
            // Guardar posición Z actual para la próxima actualización
            previousZ = position.z;
        
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
        }
        

    void Draw() {
        DrawSphere(position, radius, color);
        //DrawSphereWires(position, radius, 16, 16, BLACK);
    }

    // Resetear la pelota a una posición y velocidad inicial
    void Reset(Vector3 pos, Vector3 vel, Vector3 spn = {0.0f, 0.0f, 0.0f}) {
        position = pos;
        velocity = vel;
        spin = spn;
        isMoving = true;
        previousZ = pos.z;
    }

    // Getters
    bool GetIsMoving() const { return isMoving; }
    Vector3 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
};
#endif // BALL3D_H
