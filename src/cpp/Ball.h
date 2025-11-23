#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include <cmath>  // al inicio del archivo

class Ball {
private:
    Vector2 position;
    float radius;
    Color color;
    float speed;
    bool isMoving;
    float velocityY = 0.0f;       // Velocidad vertical
    const float gravity = 980.0f; // Gravedad en px/s^2 (ajustable)
    float restitution; // Coeficiente de restitución
    const float minHeight = 1.0f; // Altura mínima para detener la pelota

public:
    Ball(float x, float radius, float initialHeight, Color col, float spd, float rest) 
        : position({x, initialHeight}), radius(radius), color(col), speed(spd), isMoving(true), restitution(rest) {}
      
        void Update(float deltaTime, int screenHeight) {
            if (!isMoving) {
                return;
            }
        
            velocityY += gravity * deltaTime;       
            position.y += velocityY * deltaTime;
        
            float floorY = screenHeight - radius;
            if (position.y >= floorY) {
                position.y = floorY;
                velocityY = -velocityY * restitution;
        
                // stop condition
                if (fabs(velocityY) < 20.0f) { 
                    velocityY = 0.0f;
                    isMoving = false;
                }
            }
        }
    

    void Draw() {
        DrawCircle(position.x, position.y, radius, color);
        DrawCircleLines(position.x, position.y, radius, BLACK);
    }

    // Getters
    bool GetIsMoving() const { return isMoving; }
    float GetX() const { return position.x; }
    float GetY() const { return position.y; }
    float GetRadius() const { return radius; }
};

#endif // BALL_H

