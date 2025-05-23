#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
#include <string>

namespace coup {
    class Player;
    class Game {
        private:
            std::vector<Player*> players_list;
             Player* last_arrested = nullptr; //last player arrested
             size_t current_player_index;
             bool has_started;
             int active_players;
        
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
             * @brief called when a player is being eliminated from the game
             * 
             * @param p 
             */
             
            void eliminatePlayer(Player* p);

                
            /**
             * @brief Returns the name of the ACTIVE players.
             * 
             * @return std::vector<std::string> 
             */
            
            std::vector<std::string> players() const;
            
            /**
             * @brief pass the turn to the next player , or playing next turn.
             * 
             */
            void nextTurn();
            
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

            /**
             * @brief Get the Last player that has been Arrested .
             * 
             * @return Player* 
             */
            Player* getLastArrested() const;

            /**
             * @brief update the Last player that has been Arrested.
             * 
             * @return Player* 
             */
            void updateLastArrested(Player* p);
    };


};




#endif // GAME_HPP