/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

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
    sf::Text popupText;
    sf::RectangleShape popupBox;
    bool showPopup = false;           // whether any popup is shown
    bool isEndgamePopup = false;     // whether current popup is "end game"


    sf::RectangleShape gatherButton;
    sf::Text gatherText;
    std::map<std::string, sf::RectangleShape> actionButtons;
    std::map<std::string, sf::Text> actionLabels;

    Game& game;

public:
    GameScreen(Game& game);
    void run();
    bool restartRequested = false;
private:
    void renderPlayerList();
    void renderCurrentTurn();
    void renderGatherButton();
    void renderActionButtons(Player* current);
    void renderPopup();  // ← חדש
    void handleClick(const sf::Vector2f& mousePos);
    void handleActionClick(const sf::Vector2f& mousePos, Player* current);
};

} // namespace coup
