#include "Court.h"

// Constructor: recibe el ancho de la pista (la longitud se calcula con proporción real)
Court::Court(float courtWidth, float floorY) 
    : width(courtWidth), floorY(floorY) {
    // Proporciones reales de una pista de tenis: 23.77m x 10.97m ≈ 2.167:1
    length = width * (23.77f / 10.97f);
}

void Court::Draw() const {
    DrawCourtSurface();
    DrawSideLines();
    DrawBaseLines();
    DrawCenterLine();
    DrawServiceLines();
    DrawServiceSideLines();
}

void Court::DrawCourtSurface() const {
    DrawCube({width / 2.0f, floorY - 1.0f, length / 2.0f}, width, 2.0f, length, COURT_COLOR);
}

void Court::DrawSideLines() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    // Línea izquierda
    DrawCube({0.0f, lineY, length / 2.0f}, LINE_WIDTH, LINE_HEIGHT, length, LINE_COLOR);
    // Línea derecha
    DrawCube({width, lineY, length / 2.0f}, LINE_WIDTH, LINE_HEIGHT, length, LINE_COLOR);
}

void Court::DrawBaseLines() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    // Línea de fondo inferior
    DrawCube({width / 2.0f, lineY, 0.0f}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
    // Línea de fondo superior
    DrawCube({width / 2.0f, lineY, length}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
}

void Court::DrawCenterLine() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    DrawCube({width / 2.0f, lineY, length / 2.0f}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
}

void Court::DrawServiceLines() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    // Líneas de servicio (a 6.4m del centro en una pista real de 23.77m)
    float serviceLineDistance = length * (6.4f / 23.77f);  // Distancia desde el centro
    float serviceLineZ = (length / 2.0f) - serviceLineDistance;  // Posición desde el borde inferior
    
    // Línea de servicio inferior
    DrawCube({width / 2.0f, lineY, serviceLineZ}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
    // Línea de servicio superior
    DrawCube({width / 2.0f, lineY, length - serviceLineZ}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
}

void Court::DrawServiceSideLines() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    // Líneas de servicio laterales (dividen el área de servicio en dos)
    float serviceLineDistance = length * (6.4f / 23.77f);
    float serviceLineZ = (length / 2.0f) - serviceLineDistance;
    float serviceLineLength = length / 2.0f - serviceLineZ; // Longitud desde línea de servicio hasta línea central
    
    // Línea lateral de servicio inferior
    DrawCube({width / 2.0f, lineY, serviceLineZ + serviceLineLength / 2.0f}, 
             LINE_WIDTH, LINE_HEIGHT, serviceLineLength, LINE_COLOR);
    // Línea lateral de servicio superior
    DrawCube({width / 2.0f, lineY, length - serviceLineZ - serviceLineLength / 2.0f}, 
             LINE_WIDTH, LINE_HEIGHT, serviceLineLength, LINE_COLOR);
}

