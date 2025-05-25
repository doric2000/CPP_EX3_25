#include "../../include/Roles/Player.hpp"
#include <stdexcept>

namespace coup {


    Player::Player(Game& game , const std::string& name) 
    : game(game),name(name),coins_amount(0),
    active(true),extra_turn(false),is_sanctioned(false),
    mustCoup(false),cant_arrest(false),last_action(ActionType::None)
    {
                
        game.addPlayer(this); //adding a player to our game after initialization
    }

    
    std::string Player::getName() const{
        return this->name;
    }

    int Player::coins() const{
        return this->coins_amount;
    }

    void Player::incrementCoins(int c){
        this->coins_amount += c;
    }

    void Player::decrementCoins(int c){
        this->coins_amount -= c;
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

    void Player::applySanction() {
        this->is_sanctioned = true;
    }

    void Player::removeSanction() {
        this->is_sanctioned = false;
    }

    bool Player::isSanctioned() const{
        return this->is_sanctioned;
    }

    bool Player::isMustCoup() const { 
        return mustCoup; 
    }
    void Player::setMustCoup(bool v) {
        mustCoup = v; 
    }
    void Player::setCantArrest(bool p){
        cant_arrest = p;
    }
    bool Player::getCantArrestStatus() const {
        return cant_arrest;
    }
    ActionType Player::getLastAction() const {
         return last_action; 
    }
    void Player::setLastAction(ActionType a) { 
        last_action = a; 
    }
    void Player::clearLastAction() {
        last_action = ActionType::None; 
    }

    

    void Player::gather(){
        if (game.turn() != this->getName()) 
             throw std::runtime_error("Not your turn");
        if (this->isSanctioned())
            throw std::runtime_error("Player cant use Gather while being sanctioned");
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        // if there is sanction no:
        coins_amount++;
        setLastAction(ActionType::Gather);
        game.nextTurn(); 
    }

    //governador will use it different
    void Player::tax() {
        if (game.turn() != this->getName())  
            throw std::runtime_error("Not your turn");
        if (this->isSanctioned())
            throw std::runtime_error("Player cant use Tax while being sanctioned");
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        //if it hasnt been blocked:
       this->coins_amount += 2;
       setLastAction(ActionType::Tax);      
       game.nextTurn();
    }

    void Player::bribe(){
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        //check if the player has 4 coins
        if (this->coins() < 4)
            throw std::runtime_error("Not enough coins to bribe");   
        
        this->decrementCoins(4);
        if (game.dispatchBribeAttempt()){
            game.nextTurn();
            return;
        }
        setLastAction(ActionType::Bribe);
        extra_turn = true;   
    }

    void Player::arrest(Player& target) {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");

        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }

        if (getCantArrestStatus()){
            throw std::runtime_error("Spy has prevented you from using Arrest");
        }
        
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");

        // if hasnt been used twice
        if (game.getLastArrested() == &target)
        {
            throw std::runtime_error("Cannot arrest the same player twice in a row");
        }

        
        if (target.coins() == 0){
            throw std::runtime_error("Cannot arrest a Player With 0 coins.");
        }
        if (target.getName() == "Merchant")
            target.decrementCoins(2);
        else{
            target.decrementCoins(1);
            this->incrementCoins(1);
        }
        game.updateLastArrested(&target);
        setLastAction(ActionType::Arrest);  
        game.nextTurn();
    }

    void Player::sanction(Player& target){
        if (game.turn() != this->getName())  
            throw std::runtime_error("Not your turn");

        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }
        
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");
                 
        if (this->coins() < 3){
            throw std::runtime_error("Not enough coins to sanction");
        }
        if (target.isSanctioned()){ 
            throw std::runtime_error("Target already under Sanction");
        }

        this->decrementCoins(3);
        if (target.getName() == "Judge")
            this->decrementCoins(1);
        target.applySanction(); //baron will act different
        setLastAction(ActionType::Sanction);
        game.nextTurn();
    }

    //general can prevent it
    void Player::coup(Player& target){
        if (game.turn() != this->getName())  
            throw std::runtime_error("Not your turn");
        
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");

        if (this->coins() < 10)
            throw std::runtime_error("Not enough coins to Coup");

        //check who can prevent and ask them if they would like to .
        //also keep the option open.
        
        this->decrementCoins(7);
        // we make an event that will apply all general to block the coup
        if (game.dispatchCoupAttempt(target)){
            game.nextTurn();
            return;
        }
        target.eliminatePlayer();
        setLastAction(ActionType::Coup);
        game.nextTurn();
    }








};

    