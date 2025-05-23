#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>


namespace coup {


    class Player {

        protected:
            Game& game; // we should have a refernce to the game to use it for each operation.
            std::string name; // each player has a name
            int coins_amount = 0; //each player starts with 0 coins.
            bool active = true; //in indication if the player is active.
            bool extra_turn = false;
        public:
            /**
             * @brief Construct a new Player object
             * 
             * @param game - a reference to the game
             * @param name - the name of the player.
             */
            Player(Game& game , std::string& name);
        
            /**
             * @brief Destroy the Player object
             * 
             */
            virtual ~Player() = default;

            /**
             * @brief Returns the Name of the Player
             * 
             * @return std::string 
             */
            std::string getName() const;
            
            /**
             * @brief Returns the Coins of the Player
             * 
             * @return int - coins of the player.
             */
            int coins() const;

            /**
             * @brief Returns true if the player has an active extra turn
             * 
             * @return true 
             * @return false 
             */
            bool hasExtraTurn() const;


            /**
             * @brief clearing the extra turn after using it
             * 
             */
            void clearExtraTurn();

            
            /**
             * @brief Returns true if the Player is still in the game.
             * 
             * @return true - if player still plays.
             * @return false - if the player doesnt play anymore.
             */
            bool isActive() const;

            /**
             * @brief Set a player to be active.
             * 
             * @param active 
             */
            void setActive();

            /**
             * @brief eliminates the player from the game.
             * 
             */
            void eliminatePlayer();

            /**
             * @brief Returns the game object.
             * 
             * @return Game& 
             */
            Game& getGame() const;


            /********************
             * VIRTUAL FUNCTIONS
             ********************/

            /**
              * @brief Returns the role of the player.
              * 
              * @return std::string 
            */
            virtual std::string role() const = 0;

            /**
             * @brief the player gets 1 coin from the deck. 
             * this action has no cost and can be blocked by a sanction.
             * COST: no cost.
             */
            virtual void gather(); 
            
            /**
             * @brief the player gets 2 coins from the deck.
             * this action has no cost but can be blocked by some roles or actions.
             * COST: no cost
             */
            virtual void tax();    
            
            
            /**
             * @brief the player pays 4 coins to perform another action by his turn.
             * this action cant be blocked.
             * COST: 4 coins
             */
            virtual void bribe();
            
            /**
             * @brief the player picks another player and takes 1 coin from it.
             * cant be use on the same player twice.
             * COST: no cost.
             * @param target 
             */
            virtual void arrest(Player& target);
            
            /**
             * @brief the player picks another player and avoids it to use an any financial
             * actions (gather,tax) till it next turn.
             * COST: 3 coins.
             * @param target 
             */
            virtual void sanction(Player& target);

            /**
             * @brief the player picks another player and eliminates it from the game.
             * Can only be Blocked in certain conditions.
             * COST: 7 coins.
             * @param target 
             */
            virtual void coup(Player& target); 

            virtual void undo(Player& target){
                throw std::runtime_error("This role cannot undo actions");
            } 
    };


};


























#endif // PLAYER_HPP