/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#include "GUI/ReactionPopup.hpp"
#include <stdexcept>

namespace coup {

ReactionPopup::ReactionPopup(const std::string& question) {
    if (!font.loadFromFile("OpenSans.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    window.create(sf::VideoMode(500, 200), "Reaction", sf::Style::Titlebar | sf::Style::Close);

    questionText.setFont(font);
    questionText.setString(question);
    questionText.setCharacterSize(24);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition(40, 40);

    yesButton.setSize(sf::Vector2f(150, 50));
    yesButton.setFillColor(sf::Color(70, 180, 70));
    yesButton.setPosition(80, 120);

    noButton.setSize(sf::Vector2f(150, 50));
    noButton.setFillColor(sf::Color(180, 70, 70));
    noButton.setPosition(270, 120);

    yesText.setFont(font);
    yesText.setString("Yes");
    yesText.setCharacterSize(22);
    yesText.setFillColor(sf::Color::White);
    yesText.setPosition(130, 130);

    noText.setFont(font);
    noText.setString("No");
    noText.setCharacterSize(22);
    noText.setFillColor(sf::Color::White);
    noText.setPosition(325, 130);
}

bool ReactionPopup::ask() {
    window.setActive(true);
    while (window.isOpen() && !chosen) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                if (yesButton.getGlobalBounds().contains(pos)) {
                    result = true;
                    chosen = true;
                    window.close();
                }
                else if (noButton.getGlobalBounds().contains(pos)) {
                    result = false;
                    chosen = true;
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(questionText);
        window.draw(yesButton);
        window.draw(noButton);
        window.draw(yesText);
        window.draw(noText);
        window.display();

        sf::sleep(sf::milliseconds(10));
    }
    return result;
}

} // namespace coup
