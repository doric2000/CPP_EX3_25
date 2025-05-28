#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/Governor.hpp"
#include <stdexcept>

namespace coup {
    
    Governor::Governor(Game& game , const std::string& name) :
    Player(game,name)
    {
        blocked_tax = false;
    }

    Governor::~Governor() {}

    void Governor::tax() {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        if (this->isSanctioned())
            throw std::runtime_error("Player cant use Tax while being sanctioned");
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        //if it hasnt been blocked:
        incrementCoins(3);   
        setLastAction(ActionType::Tax);   
        game.nextTurn();   
    }

    void Governor::resetBlockTax(){ 
        blocked_tax = false; 
    }

    bool Governor::hasBlockedTax() const { 
        return blocked_tax; 
    }

    //can undo tax of another players
    void Governor::undo(Player& target) {
        if (game.turn() != this->getName()) 
           throw std::runtime_error("Not your turn");

        if (&target == this)
            throw std::runtime_error("Cannot undo on yourself");
        
        if (!target.isActive())
            throw std::runtime_error("Target is not active");
        
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        
        if (blocked_tax)
            throw std::runtime_error("Already used undo tax this turn");

        if (target.getLastAction() != ActionType::Tax)
            throw std::runtime_error("Can only undo if target just performed Tax");
        
        if (target.coins() < 2)
        throw std::runtime_error("Target doesn't have enough coins to reverse Tax");
        
        target.decrementCoins(2);
        target.setLastAction(ActionType::BlockTax); // that it wont be blocked again.
        
        blocked_tax = true;
    }





}