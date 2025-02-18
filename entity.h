#ifndef ENTITY_H
#define ENTITY_H

class CTransform;
class CRotation;
class CShape;
class CCollision;
class CWindowBoundsCollision;
class CScore;
class CBigEnemy;
class CLifespan;

class EntityManager;

class Entity
{
    friend EntityManager;
    
    int id = 0;
    std::string tag = "Default";
    bool alive = true;
    
    Entity() = delete;
    
    // c'tor is private, so only EntityManager can make Entity
    Entity(const std::string & tag, int id);
    
public:
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CRotation> cRotation;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CWindowBoundsCollision> cWindowBoundsCollision;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CBigEnemy> cBigEnemy;
    std::shared_ptr<CLifespan> cLifespan;
    
    Entity(const Entity & src);
    
    ~Entity();
    
    const std::string & getTag() const;
    
    void kill();
    
    const bool isAlive() const;
};

#endif
