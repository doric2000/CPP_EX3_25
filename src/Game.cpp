#include "../include/Game.hpp"
#include "../include/Roles/Player.hpp"
#include "../include/Roles/Governor.hpp"
#include "../include/Roles/Spy.hpp"
#include <stdexcept>

namespace coup {
    
    /**
     * @brief Construct - creates a new Game 
     *  player_list - empty , each time adding a player to the game adding to the list aw
     *  last_arrested - to have an indication of the last player that has been arrested
     *  current_player_index - the current player that is playing
     *  has_started - if the game has started we cant add more players.
     */
    Game::Game() 
    : players_list{} , last_arrested(nullptr) , current_player_index(0),has_started(false), active_players(0)
    {
        // already initialized everything in the list.
    }
    

    void Game::addPlayer(Player* p){
        if (has_started)
            throw std::runtime_error("Cant add a player after game has started");

        if (players_list.size() >= 6) { // cannot be more than 6 players.
            throw std::runtime_error("Maximum 6 players allowed");
        }
        for (auto* existing : players_list) { //not adding the same player twice
            if (existing == p) {
                throw std::runtime_error("Player already added");
            }
        }

        players_list.push_back(p); //push the player to the list.
        p->setActive(); // set current player to be active.
        ++active_players; //+1 to the counter of active players
    }

    void Game::eliminatePlayer(Player* p){
        p->eliminatePlayer();
        --active_players;
    }

    
    std::vector<std::string> Game::players() const {
        std::vector<std::string> names;
        names.reserve(players_list.size()); //reserve place as the player list size
        for (Player* p : players_list) {
            if (p->isActive()) {               // add only if the player is active
                names.push_back(p->getName());  // push it's name to the list
            }
        }
        return names;                         
    }

    std::string Game::turn() const{
        if (players_list.empty()) {
            throw std::runtime_error("No players in game");
        }
        Player* p = players_list[current_player_index];
        if (!p->isActive())
            throw std::runtime_error("Current player is not active");
        return p->getName();
    }


    void Game::nextTurn() {
        if (!has_started) {
            if (active_players < 2) {
                throw std::runtime_error("Need at least two players to start the game");
            }
            else
                has_started = true;
        }
        else if (has_started && active_players ==1)
        {
                //end game than return the winner still have to understand the logic.
                return;
        }
        
        if (players_list.empty()) {
            throw std::runtime_error("No players in game");
        }


        Player* p = players_list[current_player_index];
        if (p->hasExtraTurn()){
            p->clearExtraTurn();
            return;
        }

        for (int step = 1; step <= players_list.size(); ++step)
        {
            int cand = (current_player_index+step) % players_list.size();
            if (players_list[cand]->isActive()){
                current_player_index = cand;
                break;
            }
        }

        //we have to clear all blocks when moving to next turn ,
        //but we still havent implemented it yet.
        p->removeSanction();
        //if the player that has played was arrest-blocked than 
        //we will able him to use it again.
        p->setCantArrest(false);

        //if the player that has played was a governor we should able him to block tax
        //again after its turn over.
        if (auto* gov = dynamic_cast<Governor*>(p)) {
            gov->resetBlockTax();
        }
        //if the player that has played was a Spy we should able him to block arrest
        //again after its turn over.
        if (auto* spy = dynamic_cast<Spy*>(p)) {
            spy->resetArrestPreventBlock();
        }


        //if next player has 10 or more coins we will block all other actions except coup.
        Player* next = players_list[current_player_index];
        if (next->coins() >= 10) {
            next->setMustCoup(true);
        } else {
            next->setMustCoup(false);
        }
       
    }

    std::string Game::winner() const{
        if (active_players !=1)
            throw std::runtime_error("There is not a Winner yet.");
        
        for (Player* p : players_list)
        {
            if (p->isActive())
                return p->getName();
        }
        // if there is an internal error and we have accidently more than 1 active player hahaha
        throw std::runtime_error("Internal error: active_players out of sync");
    }

    Player* Game::getLastArrested() const {
        return this->last_arrested;
    }

    void Game::updateLastArrested(Player* p){
        this->last_arrested = p;
    }


}