#include "GUI/StartScreen.hpp"
#include "GUI/PlayerSelectionScreen.hpp"
#include "GUI/GameScreen.hpp"

#include "Game.hpp"
#include "Roles/Baron.hpp"
#include "Roles/General.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/Spy.hpp"

#include <vector>
#include <string>
#include <memory>
#include <random>
#include <ctime>
#include <iostream>

using namespace coup;

int main() {
    StartScreen start;
    start.run();

    if (!start.shouldStartGame()) {
        return 0;
    }

    PlayerSelectionScreen selector;
    selector.run();

    if (!selector.isGameStarted()) {
        return 0;
    }

    std::vector<std::string> names = selector.getPlayerNames();

    Game game;
    game.initializePlayers(names);

    // DEBUG - show names + roles
    std::cout << "Players in game:\n";

    for (const std::string& name : game.players()) {
        for (Player* p : game.getPlayerslist()) { // נניח שיש פונקציה כזו או שיש לך דרך להשיג את השחקנים
            if (p->getName() == name && p->isActive()) {
                std::cout << "- " << p->getName() << " as " << p->role() << "\n";
                break;
            }
        }
    }

    GameScreen screen(game);
    screen.run();

    // פה בהמשך תוכל להיכנס למסך המשחק האמיתי
    return 0;
}
