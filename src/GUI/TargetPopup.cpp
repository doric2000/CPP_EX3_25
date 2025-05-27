#include "GUI/TargetPopup.hpp"

using namespace coup;

TargetPopup::TargetPopup(Game& game, Player* current) : game(game){
    font.loadFromFile("OpenSans.ttf");

    std::vector<Player*> allPlayers = game.getPlayerslist(); 
    int height = 60 + 40 * static_cast<int>(allPlayers.size());

    window.create(sf::VideoMode(300, height), "Choose Target");

    int y = 20;
    for (Player* p : allPlayers) {
        if (!p->isActive() || p == current) continue;

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
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                for (auto& t : options) {
                    if (t.getGlobalBounds().contains(pos)) {
                        std::string name = t.getString();
                        for (const auto& target : options) {
                            if (target.getString() == name) {
                                // Find the matching Player* using name
                                // We assume Player names are unique
                                for (Player* p : game.getPlayerslist()) {
                                    if (p->getName() == name) {
                                        selectedTarget = p;
                                        break;
                                    }
                                }
                                window.close();
                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        for (auto& t : options)
            window.draw(t);
        window.display();
    }

    return selectedTarget;
}
