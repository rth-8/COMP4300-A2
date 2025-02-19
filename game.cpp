#include "game.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>

#include "component.h"
#include "entity.h"
#include "manager.h"

#include "config.h"
extern struct WindowCFG winCfg;
extern struct FontCFG fontCfg;
extern struct PlayerCFG playerCfg;
extern struct BulletCFG bulletCfg;
extern struct EnemyCFG enemyCfg;

Game::Game(std::shared_ptr<sf::RenderWindow> win)
{
    window = win;
    init();
}

void Game::init()
{
    font = std::make_shared<sf::Font>();
    if (!font->openFromFile(fontCfg.name))
    {
        std::cerr << "Failed to open font \"" << fontCfg.name << "\"!\n";
        font = nullptr;
    }

    if (font)
    {
        scoreText = std::make_shared<sf::Text>(*font);
        scoreText->setCharacterSize(fontCfg.size);
        scoreText->setFillColor(sf::Color(fontCfg.colorR, fontCfg.colorG, fontCfg.colorB));
    }
    
    manager = std::make_shared<EntityManager>();
    
    player = manager->spawnPlayer();
    
    fireDelay = 0;
    score = 0;
    
    updateScoreText();
    
    frameCnt = 0;
}

void Game::updateScoreText()
{
    if (scoreText)
    {
        std::ostringstream ss;
        ss << "Score: " << score;
        scoreText->setString(ss.str());
    }
}

void Game::sInput()
{
    if (fireDelay > 0)
        fireDelay--;
    
    if (player->cTransform)
    {
        float tmpx = player->cTransform->position.x;
        float tmpy = player->cTransform->position.y;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            if (player->cTransform)
                player->cTransform->position.y -= player->cTransform->velocity.y;
            
            if (player->cWindowBoundsCollision && (player->cTransform->position.y - player->cCollision->radius) < 0)
                player->cTransform->position.y = tmpy;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            if (player->cTransform)
                player->cTransform->position.y += player->cTransform->velocity.y;
            
            if (player->cWindowBoundsCollision && (player->cTransform->position.y + player->cCollision->radius) > winCfg.height)
                player->cTransform->position.y = tmpy;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            if (player->cTransform)
                player->cTransform->position.x -= player->cTransform->velocity.x;
            
            if (player->cWindowBoundsCollision && (player->cTransform->position.x - player->cCollision->radius) < 0)
                player->cTransform->position.x = tmpx;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            if (player->cTransform)
                player->cTransform->position.x += player->cTransform->velocity.x;
            
            if (player->cWindowBoundsCollision && (player->cTransform->position.x + player->cCollision->radius) > winCfg.width)
                player->cTransform->position.x = tmpx;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (fireDelay == 0)
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
                manager->spawnBullet(player->cTransform->position, Vec2(localPosition.x, localPosition.y));
                fireDelay = 10;
            }
        }
        else
            fireDelay = 0;
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && manager->getEntities("Special").empty())
        {
            manager->spawnSpecial(player);
        }
    }
}

void Game::sEnemySpawner()
{
    if (frameCnt % enemyCfg.spawnInterval == 0)
    {
        manager->spawnEnemy();
    }
}

void Game::sMovement()
{
    // player's movement
    if (player->cShape)
    {
        // update player's rotation & rotate
        if (player->cRotation)
        {
            player->cRotation->angle += player->cRotation->increment;
            player->cShape->shape->setRotation(sf::radians(player->cRotation->angle));
        }
        
        // move player
        if (player->cTransform)
            player->cShape->shape->setPosition(sf::Vector2f(player->cTransform->position.x, player->cTransform->position.y));
    }
    
    for (auto e : manager->getEntities("Special"))
    {
        if (player->cTransform && e->cOrbiting)
        {
            e->cOrbiting->origin.x = player->cTransform->position.x;
            e->cOrbiting->origin.y = player->cTransform->position.y;
        }
        
        if (e->cOrbiting)
        {
            e->cOrbiting->angle += e->cOrbiting->increment;
            
            e->cTransform->position.x = e->cOrbiting->origin.x + e->cOrbiting->radius * cos(e->cOrbiting->angle);
            e->cTransform->position.y = e->cOrbiting->origin.y + e->cOrbiting->radius * sin(e->cOrbiting->angle);
            
            e->cShape->shape->setPosition(sf::Vector2f(e->cTransform->position.x, e->cTransform->position.y));
        }
        
        if (e->cLifespan)
        {
            e->cLifespan->lifespan--;
            if (e->cLifespan->lifespan == 0)
            {
                e->kill();
            }
        }
    }
    
    // bullets movement
    for (auto e : manager->getEntities("Bullet"))
    {
        if (e->cRotation)
            e->cRotation->angle += e->cRotation->increment;
        
        if (e->cTransform)
            e->cTransform->position += e->cTransform->velocity;
        
        if (e->cShape)
        {
            if (e->cRotation)
                e->cShape->shape->setRotation(sf::radians(e->cRotation->angle));
            
            if (e->cTransform && e->cCollision)
            {
                e->cShape->shape->setPosition(sf::Vector2f(e->cTransform->position.x, e->cTransform->position.y));
                
                if (e->cTransform->position.x < -e->cCollision->radius || e->cTransform->position.x > winCfg.width + e->cCollision->radius ||
                    e->cTransform->position.y < -e->cCollision->radius || e->cTransform->position.y > winCfg.height + e->cCollision->radius)
                {
                    e->kill();
                }
            }
        }
    }
    
    // enemies movement
    for (auto e : manager->getEntities("Enemy"))
    {
        // update enemy's rotation
        if (e->cRotation)
            e->cRotation->angle += e->cRotation->increment;
        
        // update enemy's position
        if (e->cTransform)
            e->cTransform->position += e->cTransform->velocity;
        
        if (e->cShape)
        {
            // rotate enemy
            if (e->cRotation)
                e->cShape->shape->setRotation(sf::radians(e->cRotation->angle));
            
            // move enemy
            if (e->cTransform)
            {
                e->cShape->shape->setPosition(sf::Vector2f(e->cTransform->position.x, e->cTransform->position.y));
                
                // large enemies bouncing of windows bounds
                if (e->cWindowBoundsCollision)
                {
                    if ((e->cTransform->position.x - e->cCollision->radius) < 0 || (e->cTransform->position.x + e->cCollision->radius) > winCfg.width)
                    {
                        e->cTransform->velocity.x *= -1;
                    }
                    if ((e->cTransform->position.y - e->cCollision->radius) < 0 || (e->cTransform->position.y + e->cCollision->radius) > winCfg.height)
                    {
                        e->cTransform->velocity.y *= -1;
                    }
                }
                // small enemies travel outside of window and then dissappear
                else
                {
                    if (e->cTransform->position.x < -e->cCollision->radius || e->cTransform->position.x > winCfg.width + e->cCollision->radius ||
                        e->cTransform->position.y < -e->cCollision->radius || e->cTransform->position.y > winCfg.height + e->cCollision->radius)
                    {
                        e->kill();
                    }
                }
            }
        }
        
        // if small enemy, then fade out
        if (e->isAlive() && e->cLifespan)
        {
            e->cLifespan->lifespan--;
            if (e->cLifespan->lifespan == 0)
            {
                e->kill();
            }
            
            if (e->cShape)
            {
                sf::Color color = e->cShape->shape->getFillColor();
                color.a = ((float)e->cLifespan->lifespan / (float)enemyCfg.smallLifespan) * 255;
                e->cShape->shape->setFillColor(color);
                
                color = e->cShape->shape->getOutlineColor();
                color.a = ((float)e->cLifespan->lifespan / (float)enemyCfg.smallLifespan) * 255;
                e->cShape->shape->setOutlineColor(color);
            }
        }
    }
}

bool Game::checkCollision(std::shared_ptr<Entity> & e1, std::shared_ptr<Entity> & e2)
{
    float dist2 = 
        ((e1->cTransform->position.x - e2->cTransform->position.x) * (e1->cTransform->position.x - e2->cTransform->position.x))
        +
        ((e1->cTransform->position.y - e2->cTransform->position.y) * (e1->cTransform->position.y - e2->cTransform->position.y));
                
    float r2 = (e1->cCollision->radius + e2->cCollision->radius) * (e1->cCollision->radius + e2->cCollision->radius);
    
    if (dist2 < r2)
        return true;
    else
        return false;
}

void Game::sCollisions()
{
    for (auto e : manager->getEntities("Enemy"))
    {
        // check and process collision between player and enemy 
        if (e->isAlive() && e->cCollision && e->cTransform && player->cCollision && player->cTransform)
        {
            if (checkCollision(e, player))
            {
                if (e->cBigEnemy)
                    manager->spawnSmallEnemies(e);
                
                e->kill();
                
                // put player back to center of window
                player->cTransform->position.x = winCfg.width / 2 - playerCfg.shapeRadius / 2;
                player->cTransform->position.y = winCfg.height / 2 - playerCfg.shapeRadius / 2;
            }
        }
        
        // check and process collision between enemy and bullets
        for (auto b : manager->getEntities("Bullet"))
        {
            if (checkCollision(e, b))
            {
                if (e->cBigEnemy)
                    manager->spawnSmallEnemies(e);
                
                if (e->cScore)
                {
                    score += e->cScore->score;
                    updateScoreText();
                }

                e->kill();
                b->kill();
            }
        }
        
        for (auto spc : manager->getEntities("Special"))
        {
            if (checkCollision(e, spc))
            {
                if (e->cBigEnemy)
                    manager->spawnSmallEnemies(e);
                
                if (e->cScore)
                {
                    score += e->cScore->score;
                    updateScoreText();
                }

                e->kill();
            }
        }
    }
}

void Game::sRender()
{
    window->clear(sf::Color::Black);
    
    for (auto e : manager->getEntities())
        window->draw(*e->cShape->shape);
    
    if (scoreText)
        window->draw(*scoreText);
    
    window->display();
}

void Game::update()
{
    manager->update();
    
    sInput();
    sEnemySpawner();
    sMovement();
    sCollisions();
    sRender();
    
    frameCnt++;
}
