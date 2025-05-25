#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

namespace coup {

    class Governor : public Player {
        private:
        
        bool blocked_tax = false;
        
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
         * @brief return the name of the role of the player.
         * 
         * @return std::string 
         */
        std::string role() const {
            return "Governor";
        }

        /**
         * @brief has the governor blocked Tax for other player this turn?
         * 
         * @return true -yep
         * @return false - nope
         */
        bool hasBlockedTax() const;

        /**
         * @brief set has blocked tax to false.
         * 
         */

        void resetBlockTax();
        
        /**
         * @brief instead of getting 2 coins from the deck , a governor gets 3.
         * 
         */
        void tax() override;

        /**
         * @brief A Governor can undo tax actions of another players.
         * This Action does not spend a turn and does not have a cost.
         * @param target 
         */
        void undo(Player& target) override;
    };
}
#endif // GOVERNOR_HPP