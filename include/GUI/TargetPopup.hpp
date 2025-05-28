#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Game.hpp"
#include "Roles/Player.hpp"

namespace coup {

class TargetPopup {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<sf::Text> options;
    Player* selectedTarget = nullptr;
    Game& game;

public:
    TargetPopup(Game& game, Player* current);
    Player* chooseTarget(); // displays the window and returns the chosen target
    void draw(sf::RenderWindow& targetWindow); 
    void handleClick(const sf::Vector2f& mousePos);
};

}
