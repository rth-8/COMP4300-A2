#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>

#include "component.h"

#define PI 3.14159265358979323846

CTransform::CTransform(const Vec2 & p, const Vec2 & v) : position(p), velocity(v)
{
}

CRotation::CRotation(float ang, float inc) : angle(ang), increment(inc)
{
}

CShape::CShape(float posx, float posy, float radius, int pointCount) : radius(radius)
{
    shape = std::make_shared<sf::ConvexShape>(pointCount);
    shape->setPosition(sf::Vector2f(posx, posy));
    shape->setOrigin({posx, posy});
    
    float a = 0;
    float d = (PI*2) / pointCount;
    for (int i = 0; i < pointCount; i++)
    {
        float x = posx + radius * cos(a);
        float y = posy + radius * sin(a);
        
        std::static_pointer_cast<sf::ConvexShape>(shape)->setPoint(i, {x, y});
        
        a += d;
    }
}

CCollision::CCollision(float radius) : radius(radius)
{
}

CScore::CScore(int val) : score(val)
{
}

CLifespan::CLifespan(int val) : lifespan(val)
{
}
