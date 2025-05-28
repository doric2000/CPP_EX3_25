/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */

#include "GUI/TargetPopup.hpp"

namespace coup {

TargetPopup::TargetPopup(Game& game, Player* current)
    : game(game) {

    if (!font.loadFromFile("OpenSans.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    std::vector<Player*> allPlayers = game.getPlayerslist(); 
    int height = 60 + 40 * static_cast<int>(allPlayers.size());
    window.create(sf::VideoMode(300, height), "Choose Target");

    int y = 20;
    for (Player* p : allPlayers) {
        if (!p || !p->isActive() || p == current) continue;

        sf::Text text;
        text.setFont(font);
        text.setString(p->getName());
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(20, y);
        options.push_back(text);
        y += 40;
    }
}

Player* TargetPopup::chooseTarget() {
    window.setActive(true);
    sf::sleep(sf::milliseconds(50));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                handleClick(mousePos);
                if (selectedTarget) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Black);
        draw(window);
        window.display();
        sf::sleep(sf::milliseconds(10));
    }

    return selectedTarget;
}

void TargetPopup::handleClick(const sf::Vector2f& mousePos) {
    for (const auto& text : options) {
        if (text.getGlobalBounds().contains(mousePos)) {
            std::string name = text.getString();
            for (Player* p : game.getPlayerslist()) {
                if (p && p->getName() == name) {
                    selectedTarget = p;
                    return;
                }
            }
        }
    }
}

void TargetPopup::draw(sf::RenderWindow& targetWindow) {
    for (const auto& text : options) {
        targetWindow.draw(text);
    }
}

} // namespace coup