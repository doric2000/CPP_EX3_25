#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
#include <string>

namespace coup {
    class Player;
    class Game {
        private:
            std::vector<Player*> players_list; //a list of all players.
             Player* last_arrested = nullptr; //last player arrested
             size_t current_player_index; //which player is playing rn.
             bool has_started; // indication if the game has started already.
             int active_players; // active players counter.
             Player* lastBribed;
        
        public:
            /**
             * @brief Construct a new Game object
             * 
             */
            Game();

            /**
             * @brief Destroy all the Players that has been created.
             * 
             */
            ~Game();
            
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
             * @brief after geting all name from GUI pioks a rule to each player randomly
             * 
             * @param names 
             */
            void initializePlayers(const std::vector<std::string>& names);

                
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
             * @brief Returns the name of the winner after the current game ends.
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

            /**
             * @brief goes through all general to ask them if the would like to prevent a coup
             * 
             * @param target 
             * @return true 
             * @return false 
             */
            bool dispatchCoupAttempt(Player& target,Player& preventer);

            /**
             * @brief goes through all Judges to ask them if the would like to prevent a coup
             * 
             * @param target 
             * @return true 
             * @return false 
             */
            bool dispatchBribeAttempt(Player& target);

            /**
             * @brief Get the Players Vector object
             * 
             * @return std::vector<Player*>& 
             */
            std::vector<Player*>& getPlayerslist();

            /**
             * @brief Retur true if game is over else if not
             * 
             */
            bool isGameOver() const;


    };


};


#endif // GAME_HPP