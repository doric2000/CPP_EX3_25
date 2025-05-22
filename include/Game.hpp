#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
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
         * @brief Called from the Constructor of a rule , add a player to the list.
         * 
         * @param p 
         */
        void addPlayer(Player* p);
        
        
        std::vector<std::string> players();




    };


}












#endif // GAME_HPP