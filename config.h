#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

struct WindowCFG {
    uint16_t width;
    uint16_t height;
    uint16_t frameLimit;
    bool fullscreen;
};

struct FontCFG {
    uint16_t size;
    uint16_t colorR;
    uint16_t colorG;
    uint16_t colorB;
    std::string name;
};

struct PlayerCFG {
    uint16_t shapeRadius;
    uint16_t collisionRadius;
    uint16_t fillColorR;
    uint16_t fillColorG;
    uint16_t fillColorB;
    uint16_t outlineColorR;
    uint16_t outlineColorG;
    uint16_t outlineColorB;
    uint16_t outlineThickness;
    uint16_t numberOfVertices;
    float speed;
};

struct BulletCFG {
    uint16_t shapeRadius;
    uint16_t collisionRadius;
    uint16_t fillColorR;
    uint16_t fillColorG;
    uint16_t fillColorB;
    uint16_t outlineColorR;
    uint16_t outlineColorG;
    uint16_t outlineColorB;
    uint16_t outlineThickness;
    uint16_t numberOfVertices;
    uint16_t lifespan;
    float speed;
};

struct EnemyCFG {
    uint16_t shapeRadius;
    uint16_t collisionRadius;
    uint16_t outlineColorR;
    uint16_t outlineColorG;
    uint16_t outlineColorB;
    uint16_t outlineThickness;
    uint16_t minOfVertices;
    uint16_t maxOfVertices;
    uint16_t smallLifespan;
    uint16_t spawnInterval;
    float minSpeed;
    float maxSpeed;
};

#endif
