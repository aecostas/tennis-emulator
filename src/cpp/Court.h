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
    const Color FLOOR_COLOR = GRAY;
    const Color NET_COLOR = BLACK;
    const Color NET_POST_COLOR = DARKGRAY;
    const Color NET_BAND_COLOR = WHITE;
    const Color NET_CENTER_STRAP_COLOR = DARKGRAY;
    
    // Constantes de dimensiones de la pista (en metros)
    const float COURT_WIDTH_METERS = 10.97f;      // Ancho real de la pista
    const float COURT_LENGTH_METERS = 23.77f;    // Longitud real de la pista
    const float SERVICE_LINE_DISTANCE_METERS = 6.4f;  // Distancia de las líneas de servicio desde el centro
    
    // Constantes de dimensiones del suelo (en metros)
    const float FLOOR_EXTENSION_FRONT_BACK_METERS = 5.0f;  // Extensión del suelo en cada fondo
    const float FLOOR_EXTENSION_SIDES_METERS = 3.0f;      // Extensión del suelo en los lados
    const float FLOOR_DEPTH = 1.5f;                        // Profundidad del suelo por debajo de la pista
    const float FLOOR_HEIGHT = 1.0f;                      // Altura del suelo
    const float COURT_SURFACE_HEIGHT = 2.0f;              // Altura de la superficie de la pista
    const float COURT_SURFACE_DEPTH = 1.0f;               // Profundidad de la superficie de la pista
    
    // Constantes de dimensiones de la red (en metros)
    const float NET_POST_DISTANCE_METERS = 0.914f;        // Distancia de los postes fuera de la pista
    const float NET_HEIGHT_AT_POSTS_METERS = 1.07f;       // Altura de la red en los postes
    const float NET_HEIGHT_AT_CENTER_METERS = 0.914f;     // Altura de la red en el centro
    const float NET_POST_RADIUS_METERS = 0.05f;            // Radio del poste (5 cm)
    const float NET_BAND_HEIGHT_METERS = 0.06f;           // Altura de la cinta (6 cm)
    const float NET_BAND_THICKNESS_METERS = 0.02f;        // Grosor de la cinta (2 cm)
    const float NET_STRAP_WIDTH_METERS = 0.05f;            // Ancho del tirante (5 cm)
    const float NET_STRAP_THICKNESS_METERS = 0.02f;        // Grosor del tirante (2 cm)
    
    // Métodos privados para dibujar diferentes partes
    void DrawSurroundingFloor() const;
    void DrawCourtSurface() const;
    void DrawSideLines() const;
    void DrawBaseLines() const;
    void DrawCenterLine() const;
    void DrawServiceLines() const;
    void DrawServiceSideLines() const;
    void DrawNet() const;
    void DrawNetPosts() const;
    void DrawNetBand() const;
    void DrawNetMesh() const;
    void DrawNetCenterStrap() const;
    
    // Función auxiliar para calcular la altura de la red en cualquier punto horizontal
    float GetNetHeightAtX(float x) const;
    
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

