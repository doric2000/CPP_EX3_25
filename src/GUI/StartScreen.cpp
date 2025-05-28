/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#include "GUI/StartScreen.hpp"

StartScreen::StartScreen() : window(sf::VideoMode(600, 400), "Coup - Start") {
    font.loadFromFile("OpenSans.ttf");

    titleText.setFont(font);
    titleText.setString("Welcome to Coup");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(120, 100);

    startText.setFont(font);
    startText.setString("Click anywhere to Start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::Green);
    startText.setPosition(150, 250);
}

void StartScreen::run() {
    while (window.isOpen() && !startClicked) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
                startClicked = true;
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.draw(startText);
        window.display();
    }

    window.close();
}

bool StartScreen::shouldStartGame() const {
    return startClicked;
}