#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"

namespace coup {

    class Merchant : public Player {
        public:
            /**
             * @brief Construct a new Merchant Player
             * 
             * @param game - a reference to the game.
             * @param name - the name of the player.
             */
            Merchant(Game& game , const std::string& name);
        
            /**
             * @brief Destroy the Merchant Player
             * 
             */
            ~Merchant();
        
            /**
             * @brief Checks if the Merchant has more than 3 Coins,
             * if it does add 1 Coins.
             * COST: NO COST.
             * 
             */
            void CoinOnNewTurn();
        
    };
}








#endif // MERCHANT_HPP