#include "../../include/GUI/PlayerSelectionScreen.hpp"
#include <iostream>
#include <set>

PlayerSelectionScreen::PlayerSelectionScreen() : window(sf::VideoMode(800, 600), "Select Players") {
    font.loadFromFile("OpenSans.ttf");

    title.setFont(font);
    title.setString("Enter player names (2-6): \n (Use the arrows to navigate)");
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 30);

    instruction.setFont(font);
    instruction.setString("Press Enter to start");
    instruction.setCharacterSize(20);
    instruction.setFillColor(sf::Color::Green);
    instruction.setPosition(280, 500);

    startText.setFont(font);
    startText.setCharacterSize(24);

    for (int i = 0; i < 6; ++i) {
        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);
        label.setPosition(100, 120 + i * 60);
        label.setString("Player " + std::to_string(i + 1) + ":");
        labels.push_back(label);

        sf::RectangleShape box(sf::Vector2f(400, 35));
        box.setFillColor(sf::Color(50, 50, 50));
        box.setPosition(250, 120 + i * 60);
        inputBoxes.push_back(box);

        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::Cyan);
        inputText.setPosition(260, 125 + i * 60);
        inputTexts.push_back(inputText);

        inputs.push_back("");
    }
}

void PlayerSelectionScreen::run() {
    while (window.isOpen() && !gameStarted) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered && currentField < 6) {
                if (event.text.unicode == 8 && !inputs[currentField].empty()) {
                    inputs[currentField].pop_back();
                } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                    if (inputs[currentField].size() < 20) {
                        inputs[currentField] += static_cast<char>(event.text.unicode);
                    }
                }
                inputTexts[currentField].setString(inputs[currentField]);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    int filled = 0;
                    for (const auto& name : inputs)
                        if (!name.empty()) ++filled;

                    if (filled >= 2 && filled <= 6) {
                        std::set<std::string> uniqueNames;
                        bool hasDuplicate = false;

                        for (const auto& name : inputs) {
                            if (!name.empty()) {
                                if (!uniqueNames.insert(name).second) {
                                    hasDuplicate = true;
                                    break;
                                }
                            }
                        }

                        if (hasDuplicate) {
                            instruction.setString("Duplicate names are not allowed!");
                            instruction.setFillColor(sf::Color::Red);
                        } else {
                            gameStarted = true;
                            window.close();
                        }
                    }
                } else if (event.key.code == sf::Keyboard::Down) {
                    currentField = (currentField + 1) % 6;
                } else if (event.key.code == sf::Keyboard::Up) {
                    currentField = (currentField + 5) % 6;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(instruction);
        for (int i = 0; i < 6; ++i) {
            window.draw(labels[i]);
            window.draw(inputBoxes[i]);
            window.draw(inputTexts[i]);
        }
        window.display();
    }
}


std::vector<std::string> PlayerSelectionScreen::getPlayerNames() const {
    std::vector<std::string> result;
    for (const auto& name : inputs) {
        if (!name.empty())
            result.push_back(name);
    }
    return result;
}

bool PlayerSelectionScreen::isGameStarted() const {
    return gameStarted;
}
