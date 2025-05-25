#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"

namespace coup {

    class Judge : public Player {
        private:
            bool prevented_bribe_this_turn = false;
        public:
        /**
         * @brief Construct a new Judge Player
         * 
         * @param game - a reference to the game
         * @param name - the name of the player.
         */
        Judge(Game& game , const std::string& name);
        
        /**
         * @brief Destroy the Judge Player
         * 
         */
        ~Judge();

        /**
         * @brief return the name of the role of the player.
         * 
         * @return std::string 
         */
        std::string role() const {
            return "Judge";
        }
        
        /**
         * @brief A Judge can can undo bribe actions of another players.
         * This Action does not spend a turn and does not have a cost.
         * @param target - the player that we would like to undo his bribe action.
         */
        void PreventBribe();


        /**
         * @brief  Reset the undo bribe block on a Judge after using it and moving to the next turn.
         * 
         */
        void resetPreventBribeBlock();


        /**
         * @brief Retrurn true if this Judge has already blocked a Bribe in this turn.
         * 
         * @return true 
         * @return false 
         */
        bool hasBlockedBribeThisTurn() const;

        
    };
}

#endif // JUDGE_HPP