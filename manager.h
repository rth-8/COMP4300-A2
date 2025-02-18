#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <map>

class Vec2;
class Entity;

typedef std::vector<std::shared_ptr<Entity> > EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager
{
    EntityVector entities;
    EntityVector toBeAdded;
    EntityMap entityMap;
    int totalEntities = 0;
    
public:
    EntityManager();
    
    std::shared_ptr<Entity> addEntity(const std::string & tag);
    
    std::shared_ptr<Entity> spawnPlayer();
    std::shared_ptr<Entity> spawnBullet(const Vec2 & playerPos, const Vec2 & mousePos);
    void spawnSpecial(const Vec2 & playerPos);
    
    std::shared_ptr<Entity> spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> & bigEnemy);
    
    EntityVector & getEntities();
    EntityVector & getEntities(const std::string & tag);
    
    void update();
};

#endif
