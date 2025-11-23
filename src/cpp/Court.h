#ifndef COURT_H
#define COURT_H

#include "raylib.h"

// Clase que encapsula la pista de tenis
class Court {
private:
    float width;      // Ancho de la pista
    float length;     // Longitud de la pista
    float floorY;     // Altura del suelo
    
    // Constantes para las líneas
    const float LINE_HEIGHT = 2.0f;
    const float LINE_WIDTH = 5.0f;
    const Color LINE_COLOR = WHITE;
    const Color COURT_COLOR = DARKGREEN;
    
    // Métodos privados para dibujar diferentes partes
    void DrawCourtSurface() const;
    void DrawSideLines() const;
    void DrawBaseLines() const;
    void DrawCenterLine() const;
    void DrawServiceLines() const;
    void DrawServiceSideLines() const;
    
public:
    // Constructor: recibe el ancho de la pista (la longitud se calcula con proporción real)
    Court(float courtWidth, float floorY = 0.0f);
    
    // Dibujar toda la pista (superficie + líneas)
    void Draw() const;
    
    // Getters
    float GetWidth() const { return width; }
    float GetLength() const { return length; }
    float GetFloorY() const { return floorY; }
    float GetMaxX() const { return width; }
    float GetMaxZ() const { return length; }
};

#endif // COURT_H

