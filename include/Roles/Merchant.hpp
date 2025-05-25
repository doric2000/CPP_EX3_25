#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"

namespace coup {

    class Merchant : public Player {
        private:
            
            bool already_got_new_coin = false;
        
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
             * @brief return the name of the role of the player.
             * 
             * @return std::string 
             */
            std::string role() const {
                return "Merchant";
            }
        
            /**
             * @brief Checks if the Merchant has 3 Coins or More,
             * if it does add 1 Coins.
             * COST: NO COST.
             * 
             */
            void CoinOnNewTurn();

            /**
             * @brief when finish the current turn , reset the flag.
             * 
             */
            void reset_got_new_coin();
        
        };
}








#endif // MERCHANT_HPP