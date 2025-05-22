#ifndef BARON_HPP
#define BARON_HPP

#include "Player.hpp"


namespace coup {

    class Baron : public Player {
        public:
        /**
         * @brief Construct a new Baron Player
         * 
         * @param game - a reference to the game
         * @param name - the name of the player.
         */
        Baron(Game& game , const std::string& name);
        
        /**
         * @brief Destroy the Baron Player
         * 
         */
        ~Baron();
        
        /**
         * @brief can ivest 3 Coins and get 6 Coins back.
         * COST: 3 COINS
         * 
         */
        void invest();

        
    };
}
#endif // BARON_HPP