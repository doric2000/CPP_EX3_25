#include "../../include/Roles/Player.hpp"
#include "../../include/Game.hpp"
#include <stdexcept>

namespace coup {

    Player::Player(Game& game , std::string& name) 
    : game(game),name(name),coins_amount(0),active(true),extra_turn(false)
    {
                
        game.addPlayer(this); //adding a player to our game after initialization
    }
    
    std::string Player::getName() const{
        return this->name;
    }

    int Player::coins() const{
        return this->coins_amount;
    }

    bool Player::isActive() const{
        return this->active;
    }

    void Player::setActive(){
        active = true;
    }

    void Player::eliminatePlayer(){ // consider if should use remove player..
        active = false;
    }

    Game& Player::getGame() const{
        return this->game;
    }

    bool Player::hasExtraTurn() const{
        return this->extra_turn;
    }

    void Player::clearExtraTurn(){
        extra_turn = false;
    }
    

    void Player::gather(){
        if (game.turn() != name)
             throw std::runtime_error("Not your turn");
        // if there is sanction no:
        coins_amount++;
        game.nextTurn(); 
    }

    void Player::tax() {
        if (game.turn() != name) 
            throw std::runtime_error("Not your turn");
        //if it hasnt been blocked:
       this->coins_amount += 2;      
       game.nextTurn();
    }

    void Player::bribe(){
        if (game.turn() != name) 
            throw std::runtime_error("Not your turn");
        //check if the player has 4 coins
        if (this->coins() < 4)
            throw std::runtime_error("Not enough coins to bribe");   
        else{
            coins_amount -=4;
            extra_turn = true;
        }
            
    }

    void Player::arrest(Player& target) {
        if (game.turn() != name) 
            throw std::runtime_error("Not your turn");
        
        // if hasnt been used twice

    }








};

    