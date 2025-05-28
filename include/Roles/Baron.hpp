/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
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
         * @brief return the name of the role of the player.
         * 
         * @return std::string 
         */
        std::string role() const {
            return "Baron";
        }
        
        /**
         * @brief can ivest 3 Coins and get 6 Coins back.
         * COST: 3 COINS
         * 
         */
        void invest();

        /**
         * @brief override the applySanction function from Player , that every time a player attacks a Baron
         * , the baron will get 1 coin as a result of the sanction.
         * 
         */
        void applySanction() override;
        
    };
}
#endif // BARON_HPP