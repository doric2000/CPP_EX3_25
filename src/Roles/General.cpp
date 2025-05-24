#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/General.hpp"
#include <stdexcept>

namespace coup {
    
   General::General(Game& game , const std::string& name) :
   Player(game,name)
    {

    }

    void General::invest() {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        if (this->isMustCoup()) 
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        if (this->coins() < 3) 
            throw std::runtime_error("Player cannot invest because it doesnt have 3 coins.");
        
        this->decrementCoins(3); //to reflect the investment
        this->incrementCoins(6);
        this->setLastAction(ActionType::Invest);
        game.nextTurn();
    }

    void General::applySanction() {
        this->is_sanctioned = true;
        this->incrementCoins(1);
    }


}