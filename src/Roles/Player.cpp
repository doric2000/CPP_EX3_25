#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/Player.hpp"
#include <stdexcept>

namespace coup {

    Player::Player(Game& game , std::string& name) 
    : game(game),name(name),coins_amount(0),active(true)
    {
                
        game.addPlayer(this); //adding a player to our game after initialization
    }
    
    std::string Player::getName() const{
        return this->name;
    }

    

};

    