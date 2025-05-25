#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"

namespace coup {

    class General : public Player {
        private:
            bool prevented_coup_this_turn = false;
        
        public:
        /**
         * @brief Construct a new General Player
         * 
         * @param game - a reference to the game
         * @param name - the name of the player.
         */
        General(Game& game , const std::string& name);
        
        /**
         * @brief Destroy the General Player
         * 
         */
        ~General();

        /**
         * @brief return the name of the role of the player.
         * 
         * @return std::string 
         */
        std::string role() const {
            return "General";
        }
        
        /**
         * @brie f A general can pay 5 Coins to prevent a Coup vs another player.
         * COST: 5 Coins.
         * @param player 
         */
        void PreventCoup();

        /**
         * @brief Reset the Block of prevent a coup when already prevented and next round.
         * 
         */
        void resetPreventCoupBlock();


        /**
         * @brief Retrurn true if this general has blocked a coup already in this turn.
         * 
         * @return true 
         * @return false 
         */
        bool hasBlockedCoupThisTurn() const;
    };
}
#endif // GENERAL_HPP