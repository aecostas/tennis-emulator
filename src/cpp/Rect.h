#ifndef RECT_H
#define RECT_H

#include "raylib.h"

// Clase que encapsula el rectángulo
class Rect {
private:
    Rectangle rect;
    Color color;
    float speed;
    bool isMoving;

public:
    // Constructor
    Rect(float x, float y, float width, float height, Color col, float spd) 
        : rect({x, y, width, height}), color(col), speed(spd), isMoving(true) {}



    // Actualizar posición
    void Update(int screenWidth) {
        if (!isMoving) {
            return;
        }

        rect.x += speed;
            
        if (rect.x + rect.width >= screenWidth) {
            rect.x = screenWidth - rect.width;
            isMoving = false;
        }
    }

    void Draw() {
        DrawRectangleRec(rect, color);
        DrawRectangleLinesEx(rect, 10, BLACK);
    }

    // Getters
    bool GetIsMoving() const { return isMoving; }
    float GetX() const { return rect.x; }
    float GetY() const { return rect.y; }
};

#endif // RECT_H

