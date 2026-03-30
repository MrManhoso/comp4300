#ifndef GAME_H
#define GAME_H

#include "entity_manager.h"
#include "SFML/Graphics.hpp"

class game
{
  public:
    void init();
    void update();
    
    void movement_system();
    void user_input_system();
    void enemy_spawner_system();
    void collision_system();
    void render_system();

  private:
    sf::Window window_;
    entity_manager entities_;
    bool paused_;
    bool running_;
};

#endif