#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/General.hpp"
#include <stdexcept>

namespace coup {
    
   General::General(Game& game , const std::string& name) :
   Player(game,name)
    {
        prevented_coup_this_turn = false;
    }

    General::~General() {}


    void General::PreventCoup(){
        prevented_coup_this_turn = true;
    }

    void General::resetPreventCoupBlock()
    {
        this->prevented_coup_this_turn = false;
    }

    bool General::hasBlockedCoupThisTurn () const{
        return prevented_coup_this_turn;
    }

}