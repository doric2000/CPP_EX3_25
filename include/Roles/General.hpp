#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"

namespace coup {

    class General : public Player {
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
         * @brief A general can pay 5 Coins to prevent a Coup vs another player.
         * COST: 5 Coins.
         * @param player 
         */
        void PreventCoup(Player& target);

        
    };
}
#endif // GENERAL_HPP