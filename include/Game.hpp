#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
#include <string>

namespace coup {
    class Player;
    class Game {
        private:
            std::vector<Player*> players_list;

        public:
        /**
         * @brief Construct a new Game object
         * 
         */
        Game();
        
        /**
         * @brief Called from the Constructor of a rule , adds a player to the list.
         * 
         * @param p 
         */
        void addPlayer(Player* p);
        
        
        /**
         * @brief Returns the name of the ACTIVE players.
         * 
         * @return std::vector<std::string> 
         */
        
        std::vector<std::string> players() const;
        
        /**
         * @brief Returns the current Player that has to play;
         * 
         * @return std::string 
         */
        std::string turn() const;

        /**
         * @brief Return the name of the winner after the current game ends.
         * 
         * @return std::string 
         */
        std::string winner() const;
    };


};




#endif // GAME_HPP