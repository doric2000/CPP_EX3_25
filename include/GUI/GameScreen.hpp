#pragma once

#include <SFML/Graphics.hpp>
#include <map>

// Forward declarations
namespace coup {
    class Player;
    class Game;
}

namespace coup {

class GameScreen {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text turnLabel;
    sf::Text errorLabel;
    sf::RectangleShape gatherButton;
    sf::Text gatherText;
    std::map<std::string, sf::RectangleShape> actionButtons;
    std::map<std::string, sf::Text> actionLabels;

    Game& game;

public:
    GameScreen(Game& game);
    void run();

private:
    void renderPlayerList();
    void renderCurrentTurn();
    void renderGatherButton();
    void handleClick(const sf::Vector2f& mousePos);
    void renderActionButtons(Player* current);
    void handleActionClick(const sf::Vector2f& mousePos, Player* current);
};

} // namespace coup
