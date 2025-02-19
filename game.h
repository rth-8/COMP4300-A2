#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory.h>

class EntityManager;
class Entity;

class Game
{
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Text> scoreText;
    
    std::shared_ptr<EntityManager> manager;
    std::shared_ptr<Entity> player;
    
    bool paused;
    bool running;
    int frameCnt;
    
    int fireDelay;
    int score;
    
    void init();
    
    void sInput();
    void sEnemySpawner();
    void sMovement();
    void sCollisions();
    void sRender();
    
    void updateScoreText();
    
    bool checkCollision(std::shared_ptr<Entity> & e1, std::shared_ptr<Entity> & e2);
    
public:
    Game(std::shared_ptr<sf::RenderWindow> win);
    
    void update();
};

#endif
