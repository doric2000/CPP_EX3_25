#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace coup {

class ReactionPopup {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text questionText;
    sf::RectangleShape yesButton, noButton;
    sf::Text yesText, noText;
    bool result = false;
    bool chosen = false;

public:
    ReactionPopup(const std::string& question);
    bool ask();
};

} // namespace coup
