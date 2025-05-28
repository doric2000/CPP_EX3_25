#include "GUI/GameScreen.hpp"
#include "GUI/TargetPopup.hpp"
#include "Player.hpp"
#include "../include/Roles/Baron.hpp"
#include "../include/Roles/Spy.hpp"
#include "GUI/ReactionPopup.hpp"


#include <iostream>
#include <SFML/System.hpp>

using namespace coup;

GameScreen::GameScreen(Game& g) : game(g), window(sf::VideoMode(800, 600), "Coup - Game Screen"),isEndgamePopup(false) {
    font.loadFromFile("OpenSans.ttf");

    turnLabel.setFont(font);
    turnLabel.setCharacterSize(28);
    turnLabel.setFillColor(sf::Color::White);
    turnLabel.setPosition(20, 20);

    errorLabel.setFont(font);
    errorLabel.setCharacterSize(20);
    errorLabel.setFillColor(sf::Color::Red);
    errorLabel.setPosition(20, 500);

    popupText.setFont(font);
    popupText.setCharacterSize(22);
    popupText.setFillColor(sf::Color::White);
    popupText.setPosition(220, 280);

    popupBox.setSize(sf::Vector2f(400, 100));
    popupBox.setFillColor(sf::Color(30, 30, 30, 230));
    popupBox.setOutlineColor(sf::Color::White);
    popupBox.setOutlineThickness(2);
    popupBox.setPosition(200, 250);

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
    isEndgamePopup = false;
    showPopup = false;
    restartRequested = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                //  Case 1: Endgame popup – click restart button only
                if (showPopup && isEndgamePopup) {
                    sf::FloatRect restartRect(340, 330, 120, 40);
                    if (restartRect.contains(mousePos)) {
                        restartRequested = true;
                        window.close(); // go back to main
                        return;
                    }
                    // Clicking elsewhere in endgame popup does nothing
                    continue;
                }

                //  Case 2: Regular popup (like inspect) – close popup
                if (showPopup && !isEndgamePopup) {
                    showPopup = false;
                    continue;
                }

                //  Prevent interaction if game is already over
                if (game.isGameOver()) {
                    continue;
                }

                //  Handle regular game clicks
                Player* current = nullptr;
                for (auto* p : game.getPlayerslist())
                    if (p->getName() == game.turn()) {
                        current = p;
                        break;
                    }

                if (current) {
                    handleActionClick(mousePos, current);
                }
            }
        }

        // Rendering phase
        window.clear(sf::Color::Black);

        Player* current = nullptr;
        for (auto* p : game.getPlayerslist())
            if (p->getName() == game.turn()) {
                current = p;
                break;
            }

        renderPlayerList();
        renderCurrentTurn();
        renderActionButtons(current);

        if (showPopup)
            renderPopup();

        window.draw(errorLabel);
        window.display();

        //  Trigger endgame popup (only once)
        if (!isEndgamePopup && game.isGameOver()) {
            try {
                std::string name = game.winner();
                popupText.setString(name + " wins!");
                showPopup = true;
                isEndgamePopup = true;
            } catch (const std::exception& e) {
                errorLabel.setString(e.what());
            }
        }
    }
}




void GameScreen::renderPopup() {
    window.draw(popupBox);
    window.draw(popupText);

    if (isEndgamePopup) {
        sf::RectangleShape restartButton(sf::Vector2f(120, 40));
        restartButton.setPosition(340, 330);
        restartButton.setFillColor(sf::Color(100, 200, 100));

        sf::Text restartText;
        restartText.setFont(font);
        restartText.setString("Restart");
        restartText.setCharacterSize(18);
        restartText.setFillColor(sf::Color::Black);
        restartText.setPosition(365, 340);

        window.draw(restartButton);
        window.draw(restartText);
    }
}



void GameScreen::renderPlayerList() {
    int y = 80;
    sf::Text header;
    header.setFont(font);
    header.setCharacterSize(22);
    header.setFillColor(sf::Color::Green);
    header.setPosition(20, 45);
    header.setString("Players in game: " + std::to_string(game.getPlayerslist().size()));
    window.draw(header);

    for (Player* player : game.getPlayerslist()) {
        if (!player->isActive()) continue;

        sf::Text line;
        line.setFont(font);
        line.setCharacterSize(20);

        if (player->getName() == game.turn()) {
            line.setFillColor(sf::Color::Yellow);
            line.setStyle(sf::Text::Bold);
        } else {
            line.setFillColor(sf::Color::Cyan);
            line.setStyle(sf::Text::Regular);
        }

        line.setPosition(20, y);
        line.setString(player->getName() + " (" + player->role() + ") - Coins: " + std::to_string(player->coins()) + " | last move: " + player->actionToString(player->getLastAction()));
        window.draw(line);
        y += 30;
    }

    sf::RectangleShape separator(sf::Vector2f(2.f, 520.f));
    separator.setFillColor(sf::Color(120, 120, 120));
    separator.setPosition(500, 20);
    window.draw(separator);
}

void GameScreen::renderCurrentTurn() {
    turnLabel.setFont(font);
    turnLabel.setCharacterSize(24);
    turnLabel.setFillColor(sf::Color::White);
    turnLabel.setPosition(20, 10);
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
    showPopup = false;

    for (const auto& [action, button] : actionButtons) {
        if (button.getGlobalBounds().contains(mousePos)) {
            try {
                if (action == "gather") 
                    current->gather();
                
                else if (action == "tax")
                    current->tax();
                                
                else if (action == "bribe") {
                    try {
                        current->bribe();  
                        popupText.setString("Bribe was accepted.");
                        showPopup = true;
                        isEndgamePopup = false;
                    }
                    catch (const std::runtime_error& e) {
                        errorLabel.setString(e.what());
                    }

                }


                else if (action == "invest") {
                    if (auto* baron = dynamic_cast<Baron*>(current)) baron->invest();
                    else throw std::runtime_error("Only Baron can invest");
                }
                else if (action == "undo" || action == "arrest" || action == "sanction" || action == "prevent arrest") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    window.setActive(true);
                    if (!target) break;

                    if (action == "undo") current->undo(*target);
                    else if (action == "arrest") current->arrest(*target);
                    else if (action == "sanction") current->sanction(*target);
                    else if (action == "prevent arrest") {
                        if (auto* spy = dynamic_cast<Spy*>(current)) spy->preventArrest(*target);
                        else throw std::runtime_error("Only Spy can use prevent arrest");
                    }
                }
                else if (action == "inspect") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    window.setActive(true);
                    if (auto* spy = dynamic_cast<Spy*>(current)) {
                        if (target) {
                            int amount = spy->inspectCoins(*target);
                            popupText.setString(target->getName() + " has " + std::to_string(amount) + " coins.");
                            showPopup = true;
                            isEndgamePopup = false;
                        }
                    } else throw std::runtime_error("Only Spy can use inspect");
                }

                else if (action == "coup") {
                    TargetPopup popup(game, current);
                    Player* target = popup.chooseTarget();
                    window.setActive(true);
                    if (!target) break;

                    // Validate coup requirements BEFORE anything
                    if (current->coins() < 7)
                        throw std::runtime_error("Not enough coins to perform coup");

                    // Deduct coins BEFORE asking generals
                    current->decrementCoins(7);

                    // Ask generals if they want to block
                    if (game.dispatchCoupAttempt(*target, *current)) {
                        errorLabel.setString("Coup was blocked by a General!");
                        game.nextTurn();
                    } else {
                        current->setLastAction(ActionType::Coup); // only if executed
                        target->eliminatePlayer();
                        game.nextTurn();
                    }
                }


            } catch (const std::exception& e) {
                errorLabel.setString(e.what());
            }

            break;
        }
    }
}