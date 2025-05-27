
#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Roles/Merchant.hpp"
#include <stdexcept>

namespace coup {

   Merchant::Merchant(Game& game , const std::string& name) :
   Player(game,name)
    {
        already_got_new_coin = false;
    }

    Merchant::~Merchant() {}

    void Merchant::CoinOnNewTurn(){
        if(already_got_new_coin)
            throw std::runtime_error("Player already recieved a new coin this turn");
        
        this->incrementCoins(1);
        already_got_new_coin = true;
    }

    std::string Merchant::role() const {
        return "Merchant";
    }

    void Merchant::reset_got_new_coin(){
        already_got_new_coin = false;
    }



}



