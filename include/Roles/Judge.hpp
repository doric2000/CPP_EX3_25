#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"

namespace coup {

    class Judge : public Player {
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
         * @brief A Judge can undo a bribe action of other player,
         * what will make it lose it's 4 Coins.
         * 
         * @param target 
         */
        void undo(Player& target) override; 

        
    };
}

#endif // JUDGE_HPP