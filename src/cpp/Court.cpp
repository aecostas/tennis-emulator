#include "Court.h"
#include <cmath>

// Constructor: recibe el ancho de la pista (la longitud se calcula con proporción real)
Court::Court(float courtWidth, float floorY) 
    : width(courtWidth), floorY(floorY) {
    // Proporciones reales de una pista de tenis: 23.77m x 10.97m ≈ 2.167:1
    length = width * (COURT_LENGTH_METERS / COURT_WIDTH_METERS);
}

void Court::DrawSurroundingFloor() const {
    // El suelo se extiende más en cada fondo y en los lados
    // Escala: width unidades = COURT_WIDTH_METERS metros reales
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float EXTENSION_FRONT_BACK = FLOOR_EXTENSION_FRONT_BACK_METERS * unitsPerMeter;
    float EXTENSION_SIDES = FLOOR_EXTENSION_SIDES_METERS * unitsPerMeter;
    
    // Dimensiones del suelo completo
    float floorWidth = width + (2.0f * EXTENSION_SIDES);
    float floorLength = length + (2.0f * EXTENSION_FRONT_BACK);
    
    // Posición del centro del suelo (centrado alrededor de la pista)
    float floorCenterX = width / 2.0f;
    float floorCenterZ = length / 2.0f;
    float floorYPos = floorY - FLOOR_DEPTH;
    
    // Dibujar el suelo como un cubo plano
    DrawCube({floorCenterX, floorYPos, floorCenterZ}, floorWidth, FLOOR_HEIGHT, floorLength, FLOOR_COLOR);
}

void Court::Draw() const {
    DrawSurroundingFloor();
    DrawCourtSurface();
    DrawSideLines();
    DrawBaseLines();
    DrawCenterLine();
    DrawServiceLines();
    DrawServiceSideLines();
    DrawNet();
}

void Court::DrawCourtSurface() const {
    DrawCube({width / 2.0f, floorY - COURT_SURFACE_DEPTH, length / 2.0f}, 
             width, COURT_SURFACE_HEIGHT, length, COURT_COLOR);
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
    // Líneas de servicio
    float serviceLineDistance = length * (SERVICE_LINE_DISTANCE_METERS / COURT_LENGTH_METERS);
    float serviceLineZ = (length / 2.0f) - serviceLineDistance;
    
    // Línea de servicio inferior
    DrawCube({width / 2.0f, lineY, serviceLineZ}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
    // Línea de servicio superior
    DrawCube({width / 2.0f, lineY, length - serviceLineZ}, width, LINE_HEIGHT, LINE_WIDTH, LINE_COLOR);
}

void Court::DrawServiceSideLines() const {
    float lineY = floorY + (LINE_HEIGHT / 2.0f);
    // Líneas de servicio laterales (dividen el área de servicio en dos)
    float serviceLineDistance = length * (SERVICE_LINE_DISTANCE_METERS / COURT_LENGTH_METERS);
    float serviceLineZ = (length / 2.0f) - serviceLineDistance;
    float serviceLineLength = length / 2.0f - serviceLineZ;
    
    // Línea lateral de servicio inferior
    DrawCube({width / 2.0f, lineY, serviceLineZ + serviceLineLength / 2.0f}, 
             LINE_WIDTH, LINE_HEIGHT, serviceLineLength, LINE_COLOR);
    // Línea lateral de servicio superior
    DrawCube({width / 2.0f, lineY, length - serviceLineZ - serviceLineLength / 2.0f}, 
             LINE_WIDTH, LINE_HEIGHT, serviceLineLength, LINE_COLOR);
}

void Court::DrawNet() const {
    DrawNetPosts();
    DrawNetBand();
    DrawNetMesh();
    DrawNetCenterStrap();
}

void Court::DrawNetPosts() const {
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float postDistance = NET_POST_DISTANCE_METERS * unitsPerMeter;
    float postHeight = NET_HEIGHT_AT_POSTS_METERS * unitsPerMeter;
    float postRadius = NET_POST_RADIUS_METERS * unitsPerMeter;
    
    // Posición de la red (centro de la pista)
    float netZ = length / 2.0f;
    
    // Poste izquierdo (fuera de la pista)
    float postLeftX = -postDistance;
    float postY = floorY + (postHeight / 2.0f);
    DrawCube({postLeftX, postY, netZ}, postRadius * 2.0f, postHeight, postRadius * 2.0f, NET_POST_COLOR);
    
    // Poste derecho (fuera de la pista)
    float postRightX = width + postDistance;
    DrawCube({postRightX, postY, netZ}, postRadius * 2.0f, postHeight, postRadius * 2.0f, NET_POST_COLOR);
}

float Court::GetNetHeightAtX(float x) const {
    // Calcula la altura de la red en cualquier punto x usando dos líneas rectas
    // La red está tensa, formando dos segmentos rectos desde cada poste hasta el centro
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float postDistance = NET_POST_DISTANCE_METERS * unitsPerMeter;
    float netHeightAtPosts = NET_HEIGHT_AT_POSTS_METERS * unitsPerMeter;
    float netHeightAtCenter = NET_HEIGHT_AT_CENTER_METERS * unitsPerMeter;
    
    // Posiciones de los postes y centro
    float postLeftX = -postDistance;
    float postRightX = width + postDistance;
    float centerX = width / 2.0f;
    
    if (x <= centerX) {
        // Mitad izquierda: interpolación lineal desde poste izquierdo hasta centro
        float t = (x - postLeftX) / (centerX - postLeftX);  // t va de 0 a 1
        return netHeightAtPosts + (netHeightAtCenter - netHeightAtPosts) * t;
    } else {
        // Mitad derecha: interpolación lineal desde centro hasta poste derecho
        float t = (x - centerX) / (postRightX - centerX);  // t va de 0 a 1
        return netHeightAtCenter + (netHeightAtPosts - netHeightAtCenter) * t;
    }
}

void Court::DrawNetBand() const {
    // La cinta está tensa y forma dos líneas rectas desde cada poste hasta el centro
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float postDistance = NET_POST_DISTANCE_METERS * unitsPerMeter;
    float bandHeight = NET_BAND_HEIGHT_METERS * unitsPerMeter;
    float netZ = length / 2.0f;
    float bandWidth = NET_BAND_THICKNESS_METERS * unitsPerMeter;
    
    // Posiciones de los postes y centro
    float postLeftX = -postDistance;
    float postRightX = width + postDistance;
    float centerX = width / 2.0f;
    
    // Alturas en los postes y centro
    float netHeightAtPosts = NET_HEIGHT_AT_POSTS_METERS * unitsPerMeter;
    float netHeightAtCenter = NET_HEIGHT_AT_CENTER_METERS * unitsPerMeter;
    
    // Línea izquierda: desde el poste izquierdo hasta el centro
    float leftY = floorY + netHeightAtPosts;
    float centerY = floorY + netHeightAtCenter;
    
    // Dibujar segmentos pequeños para la línea izquierda
    const int segments = 50;
    float leftSegmentLength = centerX - postLeftX;
    for (int i = 0; i < segments; i++) {
        float t = (float)i / segments;
        float x = postLeftX + leftSegmentLength * t;
        float y = leftY + (centerY - leftY) * t;
        float segmentWidth = leftSegmentLength / segments;
        
        DrawCube({x, y, netZ}, segmentWidth, bandHeight, bandWidth, NET_BAND_COLOR);
    }
    
    // Línea derecha: desde el centro hasta el poste derecho
    float rightY = floorY + netHeightAtPosts;
    float rightSegmentLength = postRightX - centerX;
    
    // Dibujar segmentos pequeños para la línea derecha
    for (int i = 0; i < segments; i++) {
        float t = (float)i / segments;
        float x = centerX + rightSegmentLength * t;
        float y = centerY + (rightY - centerY) * t;
        float segmentWidth = rightSegmentLength / segments;
        
        DrawCube({x, y, netZ}, segmentWidth, bandHeight, bandWidth, NET_BAND_COLOR);
    }
}

void Court::DrawNetMesh() const {
    // Dibujar la red propiamente dicha como una malla de líneas para que sea transparente
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float postDistance = NET_POST_DISTANCE_METERS * unitsPerMeter;
    float netZ = length / 2.0f;
    
    // Posiciones de los postes
    float postLeftX = -postDistance;
    float postRightX = width + postDistance;
    float netWidth = postRightX - postLeftX;  // Ancho total de la red (incluyendo postes)
    
    // Número de líneas verticales y horizontales para crear la malla
    const int verticalLines = 40;   // Líneas verticales (de arriba a abajo)
    const int horizontalLines = 15; // Líneas horizontales (de lado a lado)
    
    // Dibujar líneas verticales siguiendo la forma de dos líneas rectas, desde poste a poste
    for (int i = 0; i <= verticalLines; i++) {
        float x = postLeftX + (netWidth / verticalLines) * i;
        float netHeight = GetNetHeightAtX(x);
        
        Vector3 bottom = {x, floorY, netZ};
        Vector3 top = {x, floorY + netHeight, netZ};
        DrawLine3D(bottom, top, NET_COLOR);
    }
    
    // Dibujar líneas horizontales siguiendo la forma de dos líneas rectas, desde poste a poste
    // Dividimos en segmentos para que sigan la forma
    const int horizontalSegments = 50;
    for (int i = 0; i <= horizontalLines; i++) {
        // Altura normalizada (0 = suelo, 1 = altura máxima en el centro)
        float heightRatio = (float)i / horizontalLines;
        
        // Dibujar la línea horizontal como una serie de segmentos que siguen la forma, desde poste a poste
        for (int j = 0; j < horizontalSegments; j++) {
            float x1 = postLeftX + (netWidth / horizontalSegments) * j;
            float x2 = postLeftX + (netWidth / horizontalSegments) * (j + 1);
            
            // Calcular la altura de la red en estos puntos
            float h1 = GetNetHeightAtX(x1);
            float h2 = GetNetHeightAtX(x2);
            
            // Calcular la altura objetivo basada en la proporción
            float targetH1 = h1 * heightRatio;
            float targetH2 = h2 * heightRatio;
            
            Vector3 p1 = {x1, floorY + targetH1, netZ};
            Vector3 p2 = {x2, floorY + targetH2, netZ};
            DrawLine3D(p1, p2, NET_COLOR);
        }
    }
}

void Court::DrawNetCenterStrap() const {
    // El tirante central fija la altura de la red en el centro
    float unitsPerMeter = width / COURT_WIDTH_METERS;
    float strapHeight = NET_HEIGHT_AT_CENTER_METERS * unitsPerMeter;
    float strapY = floorY + (strapHeight / 2.0f);
    float netZ = length / 2.0f;
    float strapWidth = NET_STRAP_WIDTH_METERS * unitsPerMeter;
    float strapThickness = NET_STRAP_THICKNESS_METERS * unitsPerMeter;
    
    // El tirante va desde el suelo hasta la altura de la red en el centro
    DrawCube({width / 2.0f, strapY, netZ}, strapWidth, strapHeight, strapThickness, NET_CENTER_STRAP_COLOR);
}

