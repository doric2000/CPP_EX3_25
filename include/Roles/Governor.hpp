#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

namespace coup {

    class Governor : public Player {
        public:
        /**
         * @brief Construct a new Governor Player
         * 
         * @param game - a reference to the game
         * @param name - the name of the player.
         */
        Governor(Game& game , const std::string& name);
        
        /**
         * @brief Destroy the Governor Player
         * 
         */
        ~Governor();
        
        /**
         * @brief instead of getting 2 coins from the deck , a governor gets 3.
         * 
         */
        void tax() override;

        /**
         * @brief can undo tax actions of another players.
         * 
         * @param target 
         */
        void undo(Player& target) override;
    };
}
#endif // GOVERNOR_HPP