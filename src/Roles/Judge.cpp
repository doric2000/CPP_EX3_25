/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/Judge.hpp"
#include <stdexcept>

namespace coup {
    
   Judge::Judge(Game& game , const std::string& name) :
   Player(game,name)
    {
        prevented_bribe_this_turn = false;
    }

    Judge::~Judge() {}


    void Judge::PreventBribe(){
        prevented_bribe_this_turn = true;
    }

    void Judge::resetPreventBribeBlock() {
        // Reset the block for undoing bribes
        prevented_bribe_this_turn = false;
    }
    
    // Returns true if this Judge has already blocked a Bribe in this turn.
    bool Judge::hasBlockedBribeThisTurn() const{
        return prevented_bribe_this_turn;
    }



}