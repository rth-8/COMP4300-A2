#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include <stdlib.h>
#include <math.h>

#include "manager.h"
#include "component.h"
#include "entity.h"

#define PI 3.14159265358979323846

#include "config.h"
extern struct WindowCFG winCfg;
extern struct PlayerCFG playerCfg;
extern struct BulletCFG bulletCfg;
extern struct EnemyCFG enemyCfg;

EntityManager::EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    // std::shared_ptr<Entity> e = std::make_shared<Entity>(new Entity(tag, ++totalEntities));
    auto e = std::shared_ptr<Entity>(new Entity(tag, ++totalEntities)); // hacky way of creating new entity (because make_shared can't reach private constructor of Entity)
    this->toBeAdded.push_back(e);
    return e;
}

std::shared_ptr<Entity> EntityManager::spawnPlayer()
{
    auto e = this->addEntity("Player");
    
    float posx = winCfg.width / 2 - playerCfg.shapeRadius / 2;
    float posy = winCfg.height / 2 - playerCfg.shapeRadius / 2;
    
    e->cShape = std::make_shared<CShape>(posx, posy, playerCfg.shapeRadius, playerCfg.numberOfVertices);
    
    e->cShape->shape->setFillColor(sf::Color(playerCfg.fillColorR, playerCfg.fillColorG, playerCfg.fillColorB));
    e->cShape->shape->setOutlineThickness(-playerCfg.outlineThickness);
    e->cShape->shape->setOutlineColor(sf::Color(playerCfg.outlineColorR, playerCfg.outlineColorG, playerCfg.outlineColorB));

    e->cTransform = std::make_shared<CTransform>(Vec2(posx, posy), Vec2(playerCfg.speed, playerCfg.speed));
    
    e->cRotation = std::make_shared<CRotation>(0.0f, 2*PI/180.0f);
    
    e->cCollision = std::make_shared<CCollision>(enemyCfg.collisionRadius);
    
    return e;
}

std::shared_ptr<Entity> EntityManager::spawnBullet(const Vec2 & playerPos, const Vec2 & mousePos)
{
    auto e = this->addEntity("Bullet");
    
    e->cShape = std::make_shared<CShape>(playerPos.x, playerPos.y, bulletCfg.shapeRadius, bulletCfg.numberOfVertices);
    
    e->cShape->shape->setFillColor(sf::Color(bulletCfg.fillColorR, bulletCfg.fillColorG, bulletCfg.fillColorB));
    e->cShape->shape->setOutlineThickness(-bulletCfg.outlineThickness);
    e->cShape->shape->setOutlineColor(sf::Color(bulletCfg.outlineColorR, bulletCfg.outlineColorG, bulletCfg.outlineColorB));
    
    Vec2 velocity = mousePos - playerPos;
    velocity.normalize().mul(bulletCfg.speed);
    
    e->cTransform = std::make_shared<CTransform>(playerPos, velocity);
    
    e->cRotation = std::make_shared<CRotation>(0.0f, 2*PI/180.0f);
    
    e->cCollision = std::make_shared<CCollision>(bulletCfg.collisionRadius);

    return e;
}

void EntityManager::spawnSpecial(const Vec2 & playerPos)
{

}

std::shared_ptr<Entity> EntityManager::spawnEnemy()
{
    auto e = this->addEntity("Enemy");
    
    int16_t spawnRadius = enemyCfg.shapeRadius >= enemyCfg.collisionRadius ? enemyCfg.shapeRadius : enemyCfg.collisionRadius;
    
    float posx = (rand() % (winCfg.width - 2*spawnRadius)) + spawnRadius;
    float posy = (rand() % (winCfg.height -2*spawnRadius)) + spawnRadius;
    
    int vertices = (rand() % (enemyCfg.maxOfVertices - enemyCfg.minOfVertices + 1)) + enemyCfg.minOfVertices;
    // std::cout << "vertices: " << vertices << "\n";
    
    e->cShape = std::make_shared<CShape>(posx, posy, enemyCfg.shapeRadius, vertices);
    
    e->cShape->shape->setFillColor(sf::Color(rand()%200+55, rand()%200+55, rand()%200+55));
    e->cShape->shape->setOutlineThickness(-enemyCfg.outlineThickness);
    e->cShape->shape->setOutlineColor(sf::Color(enemyCfg.outlineColorR, enemyCfg.outlineColorG, enemyCfg.outlineColorB));
    
    int16_t smin = enemyCfg.minSpeed * 100;
    int16_t smax = enemyCfg.maxSpeed * 100;
    float sx = ((rand() % (smax - smin + 1)) + smin) / 100.0f;
    float sy = ((rand() % (smax - smin + 1)) + smin) / 100.0f;
    // std::cout << "speed: " << sx << "," << sy << "\n";
    e->cTransform = std::make_shared<CTransform>(Vec2(posx, posy), Vec2(sx, sy));
    
    e->cRotation = std::make_shared<CRotation>(0.0f, 2*PI/180.0f);
    
    e->cCollision = std::make_shared<CCollision>(enemyCfg.collisionRadius);
    
    e->cScore = std::make_shared<CScore>(100 * vertices);
    
    e->cBigEnemy = std::make_shared<CBigEnemy>();
    
    e->cWindowBoundsCollision = std::make_shared<CWindowBoundsCollision>();
    
    return e;
}

void EntityManager::spawnSmallEnemies(std::shared_ptr<Entity> & bigEnemy)
{
    // std::cout << "MANAGER: spawnSmallEnemies\n";
    
    int pointCount = std::static_pointer_cast<sf::ConvexShape>(bigEnemy->cShape->shape)->getPointCount();
    
    float a = 0;
    float d = (PI*2) / pointCount;
    for (int i = 0; i < pointCount; i++)
    {
        float x = bigEnemy->cTransform->position.x + bigEnemy->cShape->radius * cos(a);
        float y = bigEnemy->cTransform->position.y + bigEnemy->cShape->radius * sin(a);
        
        auto smallEnemy = this->addEntity("Enemy");
        smallEnemy->cShape = std::make_shared<CShape>(x, y, bigEnemy->cShape->radius / 2, pointCount);
        
        smallEnemy->cShape->shape->setFillColor(bigEnemy->cShape->shape->getFillColor());
        smallEnemy->cShape->shape->setOutlineThickness(bigEnemy->cShape->shape->getOutlineThickness());
        smallEnemy->cShape->shape->setOutlineColor(bigEnemy->cShape->shape->getOutlineColor());
        
        Vec2 pos(x, y);
        Vec2 velocity = pos - bigEnemy->cTransform->position;
        velocity.normalize().mul(bigEnemy->cTransform->velocity.mag());
        
        smallEnemy->cTransform = std::make_shared<CTransform>(pos, velocity);
        
        smallEnemy->cRotation = std::make_shared<CRotation>(0.0f, 2*PI/180.0f);
    
        smallEnemy->cCollision = std::make_shared<CCollision>(bigEnemy->cCollision->radius / 2);
        
        smallEnemy->cScore = std::make_shared<CScore>(100 * pointCount * 2);
        
        smallEnemy->cBigEnemy = nullptr;
        smallEnemy->cWindowBoundsCollision = nullptr;
        
        smallEnemy->cLifespan = std::make_shared<CLifespan>(enemyCfg.smallLifespan);
        
        a += d;
    }
}

EntityVector & EntityManager::getEntities()
{
    return this->entities;
}
    
EntityVector & EntityManager::getEntities(const std::string & tag)
{
    return this->entityMap[tag];
}
    
void EntityManager::update()
{
    // std::cout << "MANAGER: update\n";
    
    // https://en.cppreference.com/w/cpp/container/vector/erase
    
    for (auto it = this->entities.begin(); it != this->entities.end();)
    {
        if ((*it)->isAlive() == false)
        {
            for (auto it2 = this->entityMap[(*it)->getTag()].begin(); it2 != this->entityMap[(*it)->getTag()].end();)
            {
                if ((*it2)->isAlive() == false)
                {
                    // std::cout << "MANAGER: Delayed removing of entity from map\n";
                    this->entityMap[(*it)->getTag()].erase(it2);
                }
                else
                    ++it2;
            }
            
            // std::cout << "MANAGER: Delayed removing of entity from vector (" << (*it)->getTag() << ")\n";
            it = this->entities.erase(it);
        }
        else
            ++it;
    }
    
    for (auto e : this->toBeAdded)
    {
        // std::cout << "MANAGER: Delayed adding of entity (" << e->getTag() << ")\n";
        this->entities.push_back(e);
        this->entityMap[e->getTag()].push_back(e);
    }
    this->toBeAdded.clear();
}
