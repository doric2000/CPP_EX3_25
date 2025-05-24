#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup {

    class Spy : public Player {
        private:
            
            bool prevent_arrest_next_turn= false;
        
        public:
        /**
         * @brief Construct a new Spy Player
         * 
         * @param game - a reference to the game
         * @param name - the name of the player.
         */
        Spy(Game& game , const std::string& name);
        
        /**
         * @brief Destroy the Spy Player
         * 
         */
        ~Spy();
        
        /**
         * @brief A spy can see the coins amount of all other players, 
         * it doesnt count as a turn.
         * COST: no cost.
         * 
         * @param target 
         * @return int 
         */
        int inspectCoins(const Player& target) const;

        /**
         * @brief A spy can prevent other player to use arrest action in it next turn.
         * COST: no cost.
         * @param target - the player that we would like to prevent from using arrest.
         */
        void preventArrest(Player& target);


        void resetArrestPreventBlock()
        {
            this->prevent_arrest_next_turn = false;
        }

    };
}
#endif // SPY_HPP