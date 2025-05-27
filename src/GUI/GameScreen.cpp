#include "GUI/GameScreen.hpp"
#include "GUI/TargetPopup.hpp"
#include "Player.hpp"
#include "../include/Roles/Baron.hpp"

#include <iostream>

using namespace coup;

GameScreen::GameScreen(Game& g) : game(g), window(sf::VideoMode(800, 600), "Coup - Game Screen") {
    font.loadFromFile("OpenSans.ttf");

    turnLabel.setFont(font);
    turnLabel.setCharacterSize(28);
    turnLabel.setFillColor(sf::Color::White);
    turnLabel.setPosition(20, 20);

    errorLabel.setFont(font);
    errorLabel.setCharacterSize(20);
    errorLabel.setFillColor(sf::Color::Red);
    errorLabel.setPosition(20, 500);

    gatherButton.setSize(sf::Vector2f(150, 50));
    gatherButton.setFillColor(sf::Color(70, 130, 180));
    gatherButton.setPosition(325, 300);

    gatherText.setFont(font);
    gatherText.setString("Gather");
    gatherText.setCharacterSize(22);
    gatherText.setFillColor(sf::Color::White);
    gatherText.setPosition(355, 310);
}

void GameScreen::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                Player* current = nullptr;
                for (auto* p : game.getPlayerslist())
                    if (p->getName() == game.turn()) current = p;

                handleActionClick(mousePos, current);
            }
        }

        Player* current = nullptr;
        for (auto* p : game.getPlayerslist())
            if (p->getName() == game.turn()) current = p;

        window.clear(sf::Color::Black);
        renderPlayerList();
        renderCurrentTurn();
        renderActionButtons(current);
        window.draw(errorLabel);
        window.display();
    }
}


void GameScreen::renderPlayerList() {
    int y = 80; // היה 70, כעת נמוך טיפה כדי להתרחק מהכותרת

    sf::Text header;
    header.setFont(font);
    header.setCharacterSize(22);
    header.setFillColor(sf::Color::Green);
    header.setPosition(20, 45); // הכותרת של רשימת השחקנים
    header.setString("Players in game: " + std::to_string(game.getPlayerslist().size()));
    window.draw(header);

    for (Player* player : game.getPlayerslist()) {
        if (!player->isActive()) continue;

        sf::Text line;
        line.setFont(font);
        line.setCharacterSize(20);

        //if its the current player's turn then mark it pls
        if (player->getName() == game.turn()) {
            line.setFillColor(sf::Color::Yellow);
            line.setStyle(sf::Text::Bold);
        }    
        else {
        line.setFillColor(sf::Color::Cyan);
        line.setStyle(sf::Text::Regular);
    }
        line.setPosition(20, y);
        line.setString(player->getName() + " (" + player->role() + ") - Coins: " + std::to_string(player->coins()));
        window.draw(line);
        y += 30;
    }

    // קו מפריד – נסדר אותו לגובה נכון
    sf::RectangleShape separator(sf::Vector2f(2.f, 520.f)); // היה 500, נמתח יותר
    separator.setFillColor(sf::Color(120, 120, 120));
    separator.setPosition(500, 20); // תואם למעלה יותר
    window.draw(separator);
}



void GameScreen::renderCurrentTurn() {
    turnLabel.setFont(font);
    turnLabel.setCharacterSize(24);
    turnLabel.setFillColor(sf::Color::White);
    turnLabel.setPosition(20, 10); // מקודם היה על 30 — מוקדם יותר כדי לא להידחף
    turnLabel.setString("Current turn: " + game.turn());
    window.draw(turnLabel);
}

void GameScreen::renderGatherButton() {
    window.draw(gatherButton);
    window.draw(gatherText);
}


void GameScreen::renderActionButtons(Player* current) {
    actionButtons.clear();
    actionLabels.clear();

    std::vector<std::string> actions = {
        "gather", "tax", "bribe", "arrest", "sanction", "coup"
    };

    if (current->role() == "Governor") actions.push_back("undo");
    else if (current->role() == "Spy") {
        actions.push_back("inspect");
        actions.push_back("prevent arrest");
    }
    else if (current->role() == "Baron") actions.push_back("invest");

    int y = 100;
    int x = 550;

    for (const std::string& action : actions) {
        sf::RectangleShape button(sf::Vector2f(160, 40));
        button.setFillColor(sf::Color(100, 100, 150));
        button.setPosition(x, y);
        actionButtons[action] = button;

        sf::Text label;
        label.setFont(font);
        label.setString(action);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
        label.setPosition(x + 10, y + 8);
        actionLabels[action] = label;

        y += 50;
    }

    for (const auto& [_, btn] : actionButtons)
        window.draw(btn);
    for (const auto& [_, lbl] : actionLabels)
        window.draw(lbl);
}





void GameScreen::handleActionClick(const sf::Vector2f& mousePos, Player* current) {
    errorLabel.setString("");

    for (const auto& [action, button] : actionButtons) {
        if (button.getGlobalBounds().contains(mousePos)) {
            try {
                if (action == "gather") {
                    current->gather();
                }
                else if (action == "tax") {
                    current->tax();
                }
                else if (action == "bribe") {
                    current->bribe();
                }
                else if (action == "invest") {
                    if (auto* baron = dynamic_cast<Baron*>(current)) {
                        baron->invest();
                    } else {
                        throw std::runtime_error("Only Baron can invest");
                    }
                }
                else if (action == "undo") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    if (target) {
                        current->undo(*target);
                    }
                }
                else if (action == "coup") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    if (target) {
                        current->coup(*target); 
                    }
                }
                else if (action == "arrest") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    if (target) {
                        current->arrest(*target);
                    }
                }
                else if (action == "sanction") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    if (target) {
                        current->sanction(*target);
                    }
                }

            } catch (const std::exception& e) {
                errorLabel.setString(e.what());
            }

            break;
        }
    }
}

