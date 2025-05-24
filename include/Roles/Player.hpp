#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>


namespace coup {

    enum class ActionType {
    None,
    Gather,
    Tax,
    Bribe,
    Arrest,
    Sanction,
    Coup,
    Invest,
    BlockTax,
    BlockArrest
};

    class Player {

        protected:
            Game& game; // we should have a refernce to the game to use it for each operation.
            std::string name; // each player has a name
            int coins_amount = 0; //each player starts with 0 coins.
            bool active = true; //in indication if the player is active.
            bool extra_turn = false; // to know if the player has an extra turn.
            bool is_sanctioned = false; // if the player is sanctioned
            bool mustCoup = false; // if has 10 or more coins must do a coup.
            bool cant_arrest = false; // if a spy prevented from a player use arrest action.
            ActionType last_action = ActionType::None; // to have an indication of last action of each player
        public:
            /**
             * @brief Construct a new Player object
             * 
             * @param game - a reference to the game
             * @param name - the name of the player.
             */
            Player(Game& game , const std::string& name);
        
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
             * @brief add c coins to the current Player balance.
             * 
             * @param c - coins to add
             */
            void incrementCoins(int c);


            /**
             * @brief takes c coins from the current Player balance.
             * 
             * @param c - coins to take
             */
            void decrementCoins(int c);


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
             * @brief Returns true if the player has a sanction.
             * 
             * @return true - if has
             * @return false - if does not have.
             */
            bool isSanctioned() const;
        

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

            /**
             * @brief Return if the player must do a coup if has 10 or more Coins.
             * 
             * @return true - if must coup
             * @return false - if there is no necessery.
             */
             
            bool isMustCoup() const;

            /**
             * @brief Set the Must Coup;
             * 
             * @param c - must coup or not :D
             */
            void setMustCoup(bool c);

            /**
             * @brief if player has been blocked from arresting by a spy set to true
             * else - false.
             * @param p - true or false by argument,
             */
            void setCantArrest(bool p);

            /**
             * @brief Get the Cant Arrest Status object
             * 
             * @return true - if can arrest and not blocked by a spy
             * @return false  - if cannot arrest and blocked by a spy
             */
            bool getCantArrestStatus() const;

            /**
             * @brief Get the last action of a player.
             * 
             * @return ActionType 
             */
            ActionType getLastAction() const;

            /**
             * @brief Set the Last Action of a player to a
             * 
             * @param a - last action.
             */
            void setLastAction(ActionType a);

            /**
             * @brief clear the last action of a player (None)
             * 
             */
            void clearLastAction();

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
             * most be done if a player has 10 or more Coins.
             * COST: 7 coins.
             * PREVENT: General can prevent for 5 Coins , agains him or other player.
             * @param target - who will be eliminated :D
             */
            virtual void coup(Player& target); 

            /**
             * @brief undo a specific action.
             * USE_BY: judge can undo a bribe , Governor can undo tax.
             * @param target - the player to undo its action.
             */
            virtual void undo(Player& target){
                throw std::runtime_error("This role cannot undo actions");
            } 

            /**
             * @brief Applies sanction on a player , we will have to modify it on baron.
             * 
             */
            virtual void applySanction();


            /**
             * @brief removes sanction on a player , we will have to modify it no baron.
             * 
             */
            void removeSanction();
    };

}
#endif // PLAYER_HPP