#include "../../include/Roles/Player.hpp"
#include <stdexcept>
#include <iostream>

namespace coup {


    Player::Player(Game& game , const std::string& name) 
    : game(game),name(name),coins_amount(0),
    active(true),extra_turn(false),is_sanctioned(false),
    mustCoup(false),cant_arrest(false),last_action(ActionType::None)
    {
                
        game.addPlayer(this); //adding a player to our game after initialization
    }

    Player::~Player() {
    
    }

    std::string Player::role() const {
        return "Player";
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
        if (this->coins() - c < 0 )
            throw std::runtime_error("A player cannot have less than 0 coins.");
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


   void Player::undo(Player& target){
        throw std::runtime_error("This role cannot undo actions");
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

    void Player::bribe() {
        // 1. Prevent two bribes in the same turn (even before any coin logic)
        if (bribedThisTurn)
            throw std::runtime_error("Cannot use bribe twice in the same turn");

        // 2. Ensure it's this player's turn
        if (game.turn() != this->getName())
            throw std::runtime_error("Not your turn");

        // 3. Must-coup rule
        if (this->isMustCoup())
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");

        // 4. Check funds
        if (this->coins() < 4)
            throw std::runtime_error("You must have at least 4 coins to use Bribe");

        // 5. Charge the coins up-front
        this->decrementCoins(4);

        // 6. Mark that we attempted a bribe *before* any dispatch
        this->setBribedThisTurn();

        // 7. Let Judges attempt to block
        bool blocked = game.dispatchBribeAttempt(*this);
        if (blocked) {
            // coins already spent, and we won't re-enter here because bribedThisTurn is true
            throw std::runtime_error("Bribe was blocked by a Judge");
        }

        // 8. Bribe succeeded: record action and grant extra turn
        setLastAction(ActionType::Bribe);
        extra_turn = true;
    }




    void Player::arrest(Player& target) {
        if (game.turn() != this->getName()) 
            throw std::runtime_error("Not your turn");
        
        if (this->isMustCoup()) {
            throw std::runtime_error("Player must perform coup when holding 10 or more coins");
        }

        if (&target == this)
        throw std::runtime_error("Cannot arrest yourself");



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
        if (target.role() == "Merchant"){
            if (target.coins() < 2)
                throw std::runtime_error("Cannot arrest a Merchant With 1 coin.");
            target.decrementCoins(2);
        }
        else if (target.role() == "General"){
            this->incrementCoins(1);
        }

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

        if (&target == this)
            throw std::runtime_error("Cannot sanction yourself");

        
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");
                 
        if (target.isSanctioned()){ 
            throw std::runtime_error("Target already under Sanction");
        }
            // Check coins requirement based on target type
        if (target.role() == "Judge") {
            if (target.coins() < 4) {
                throw std::runtime_error("Cannot sanction a Judge With less than 4 coins.");
            }
            if (this->coins() < 4) { // Need 4 coins total for Judge (3 + 1)
                throw std::runtime_error("Not enough coins to sanction a Judge");
            }
            this->decrementCoins(4); // Deduct all 4 coins at once for Judge
        } else {
            if (this->coins() < 3) {
                throw std::runtime_error("Not enough coins to sanction");
            }
            this->decrementCoins(3);
        }

        target.applySanction(); //baron will act different
        setLastAction(ActionType::Sanction);
        game.nextTurn();
    }

    //general can prevent it
    void Player::coup(Player& target) {
        if (game.turn() != this->getName())  
            throw std::runtime_error("Not your turn");

        if (&target == this)
            throw std::runtime_error("Cannot coup yourself");
        
        if (!target.isActive()) 
            throw std::runtime_error("Target is not active");

        target.eliminatePlayer();
        setLastAction(ActionType::Coup);
    }







};

    