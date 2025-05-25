#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/Spy.hpp"
#include <stdexcept>

namespace coup {
    
   Spy::Spy(Game& game , const std::string& name) :
   Player(game,name)
    {
        prevent_arrest_next_turn = false;
    }

    Spy::~Spy() {}

    int Spy::inspectCoins(const Player& target) const {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        if (this->isMustCoup()) 
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");
        
        return target.coins();
    }


    //can prevent other player from using Arrest action.
    void Spy::preventArrest(Player& target) {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        
        if (this->isMustCoup()) 
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        
        if (!target.isActive())
            throw std::runtime_error("Target is not active");
        

        if (prevent_arrest_next_turn)
            throw std::runtime_error("Player cant Prevent arrest twice.");

        if (&target == this)
            throw std::runtime_error("Cannot prevent your own arrest");

        if (target.getCantArrestStatus()){
            throw std::runtime_error("Another player has already blocked arrest for this player this round.");
        }
        
        
        target.setCantArrest(true);
        prevent_arrest_next_turn = true;
    }





}