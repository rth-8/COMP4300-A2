#ifndef COMPONENT_H
#define COMPONENT_H

#include "vector2d.h"

class CTransform
{
public:
    Vec2 position = { 0, 0 };
    Vec2 velocity = { 0, 0 };
    
    CTransform() = delete;
    CTransform(const Vec2 & p, const Vec2 & v);
};

class CRotation
{
public:
    float angle = 0.0f;
    float increment = 0.0f;
    
    CRotation() = delete;
    CRotation(float ang, float inc);
};

class CShape
{
public:
    std::shared_ptr<sf::Shape> shape;
    float radius;
    
    CShape() = delete;
    CShape(float posx, float posy, float radius, int pointCount);
};

class CCollision
{
public:
    float radius;
    
    CCollision() = delete;
    CCollision(float radius);
};

class CWindowBoundsCollision
{
};

class CScore
{
public:
    int score;
    
    CScore() = delete;
    CScore(int val);
};

class CBigEnemy
{
};

class CLifespan
{
public:
    int lifespan;
    
    CLifespan() = delete;
    CLifespan(int val);
};

#endif
