/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#pragma once
#include <SFML/Graphics.hpp>

class StartScreen {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text startText;
    bool startClicked = false;

public:
    StartScreen();
    void run();                // opens the window and handles events
    bool shouldStartGame() const; // returns true if user clicked start
};