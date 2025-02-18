#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <time.h>

#include "config.h"
struct WindowCFG winCfg;
struct FontCFG fontCfg;
struct PlayerCFG playerCfg;
struct BulletCFG bulletCfg;
struct EnemyCFG enemyCfg;

#include "game.h"

void load_configuration()
{
    std::fstream input;

    input.open("config.dat", std::ios::in);
    if (input.is_open())
    {
        std::string line;
        while (std::getline(input, line))
        {
            // std::cout << "LINE: " << line << '\n';
            
            if (line[0] == '#')
                continue;
            
            std::stringstream ss(line);
            std::string token;
            
            if (std::getline(ss, token, ' '))
            {
                if (token == "Window")
                {
                    std::cout << "Reading " << token << "...\n";
                    ss >> winCfg.width >> winCfg.height >> winCfg.frameLimit >> winCfg.fullscreen;
                }
                else
                if (token == "Font")
                {
                    std::cout << "Reading " << token << "...\n";
                    ss >> fontCfg.name >> fontCfg.size >> fontCfg.colorR >> fontCfg.colorG >> fontCfg.colorB;
                }
                else
                if (token == "Player")
                {
                    std::cout << "Reading " << token << "...\n";
                    ss >> playerCfg.shapeRadius
                       >> playerCfg.collisionRadius
                       >> playerCfg.speed
                       >> playerCfg.fillColorR
                       >> playerCfg.fillColorG
                       >> playerCfg.fillColorB
                       >> playerCfg.outlineColorR
                       >> playerCfg.outlineColorG
                       >> playerCfg.outlineColorB
                       >> playerCfg.outlineThickness
                       >> playerCfg.numberOfVertices;
                }
                else
                if (token == "Bullet")
                {
                    std::cout << "Reading " << token << "...\n";
                    ss >> bulletCfg.shapeRadius
                       >> bulletCfg.collisionRadius
                       >> bulletCfg.speed
                       >> bulletCfg.fillColorR
                       >> bulletCfg.fillColorG
                       >> bulletCfg.fillColorB
                       >> bulletCfg.outlineColorR
                       >> bulletCfg.outlineColorG
                       >> bulletCfg.outlineColorB
                       >> bulletCfg.outlineThickness
                       >> bulletCfg.numberOfVertices
                       >> bulletCfg.lifespan;
                }
                else
                if (token == "Enemy")
                {
                    std::cout << "Reading " << token << "...\n";
                    ss >> enemyCfg.shapeRadius
                       >> enemyCfg.collisionRadius
                       >> enemyCfg.minSpeed
                       >> enemyCfg.maxSpeed
                       >> enemyCfg.outlineColorR
                       >> enemyCfg.outlineColorG
                       >> enemyCfg.outlineColorB
                       >> enemyCfg.outlineThickness
                       >> enemyCfg.minOfVertices
                       >> enemyCfg.maxOfVertices
                       >> enemyCfg.smallLifespan
                       >> enemyCfg.spawnInterval;
                }
                else
                {
                    std::cerr << "Unknown type '" << token << "'!\n";
                }
            }
        }

        input.close();
    }
}

int main()
{
    std::cout << "START\n";
    
    srand(time(NULL));
    
    load_configuration();
    
    sf::State state = (winCfg.fullscreen == true) ? sf::State::Fullscreen : sf::State::Windowed;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode({winCfg.width, winCfg.height}), "COMP4300 A2", sf::Style::Close, state);
    window->setFramerateLimit(winCfg.frameLimit);
    
    Game game(window);
    
    bool aboutToClose = false;
    
    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
                aboutToClose = true;
            }
        }
        
        if (aboutToClose)
            break;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window->close();
            break;
        }
        
        game.update();
    }
    
    std::cout << "END\n";
    return 0;
}
