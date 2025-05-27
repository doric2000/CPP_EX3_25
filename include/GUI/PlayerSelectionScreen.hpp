#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PlayerSelectionScreen {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text title;
    sf::Text instruction;
    sf::Text startText;
    std::vector<sf::Text> labels;
    std::vector<std::string> inputs;
    std::vector<sf::RectangleShape> inputBoxes;
    std::vector<sf::Text> inputTexts;
    int currentField = 0;
    bool gameStarted = false;

public:
    PlayerSelectionScreen();
    void run();
    std::vector<std::string> getPlayerNames() const;
    bool isGameStarted() const;
};