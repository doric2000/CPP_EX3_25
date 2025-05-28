/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#include "../include/Game.hpp"
#include "../include/Roles/Player.hpp"
#include "../include/Roles/Governor.hpp"
#include "../include/Roles/Spy.hpp"
#include "../include/Roles/General.hpp"
#include "../include/Roles/Judge.hpp"
#include "../include/Roles/Merchant.hpp"
#include "../include/Roles/Baron.hpp"
#include "../include/GUI/ReactionPopup.hpp"

#include <random>
#include <ctime>
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
    : players_list{} , last_arrested(nullptr) , current_player_index(0),has_started(false), active_players(0),lastBribed(nullptr)
    {
        // already initialized everything in the list.
    }

    Game::~Game() {
        for (Player* p : players_list) {
            delete p;
        }
        players_list.clear();
    }

    std::vector<Player*>& Game::getPlayerslist() {
        return players_list;
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


    void Game::initializePlayers(const std::vector<std::string>& names) {
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 5);

    for (const std::string& name : names) {
        int roleIndex = dist(rng);
        switch (roleIndex) {
            case 0: new Merchant(*this, name); break;
            case 1: new Spy(*this, name); break;
            case 2: new Baron(*this, name); break;
            case 3: new Governor(*this, name); break;
            case 4: new Judge(*this, name); break;
            case 5: new General(*this, name); break;
        }
    }
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
        
        if (players_list.empty()) {
            throw std::runtime_error("No players in game");
        }


        Player* p = players_list[current_player_index];
        if (p->hasExtraTurn()){
            p->clearExtraTurn();
            return;
        }



        for (size_t step = 1; step <= players_list.size(); ++step)
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
        //if the player that has played was a General we should able him to prevent a coup 
        //again after its turn over.
        if (auto* Gen = dynamic_cast<General*>(p)) {
            Gen->resetPreventCoupBlock();
        }
        //if the player that has played was a Judge we should able him to prevent a Bribe 
        //again after its turn over.
        if (auto* jud = dynamic_cast<Judge*>(p)) {
            jud->resetPreventBribeBlock();
        }
        
         //if the player that has played was a Merchant we should able him to take a new Coins
        //again if has >=3 coins on its next turn.
        if (auto* mer = dynamic_cast<Merchant*>(p)) {
            mer->reset_got_new_coin();
        }

        //if next player has 10 or more coins we will block all other actions except coup.
        Player* next = players_list[current_player_index];
        if (next->coins() >= 10) {
            next->setMustCoup(true);
        } else {
            next->setMustCoup(false);
        }
        // checks if the player to be play is a Merchant , if it is we should add it an Extra Coin.
        if (auto* n = dynamic_cast<Merchant*>(next)) {
            if (n->coins()>=3)
            n->CoinOnNewTurn();
        }

        //clear next action in the new player to be played and clear bribe this turn flag
        next->clearBribedThisTurn();
        next->clearLastAction();


       
    }

    std::string Game::winner() const {
        std::string lastName = "";
        int count = 0;

        for (Player* p : players_list) {
            if (p->isActive()) {
                lastName = p->getName();
                ++count;
            }
        }

        if (count != 1)
            throw std::runtime_error("There is not a Winner yet.");

        return lastName;
    }


    Player* Game::getLastArrested() const {
        return this->last_arrested;
    }

    void Game::updateLastArrested(Player* p){
        this->last_arrested = p;
    }

    bool Game::dispatchCoupAttempt(Player& target, Player& preventer) {
        if (!target.isActive())
            throw std::runtime_error("Target is not active");

        if (&target == &preventer)
            throw std::runtime_error("Cannot prevent coup on yourself");

        for (Player* p : players_list) {
            if (!p->isActive() || p == &preventer) continue;

            auto* gen = dynamic_cast<General*>(p);
            if (!gen) continue;

            if (gen->coins() < 5 || gen->hasBlockedCoupThisTurn()) continue;

            std::string question = "General " + gen->getName() +
                ": Block Coup on " + target.getName() + " for 5 coins?";
            ReactionPopup popup(question);

            if (popup.ask()) {
                gen->PreventCoup();
                gen->decrementCoins(5);
                return true;
            }
        }

        return false;
    }


    bool Game::dispatchBribeAttempt(Player& target) {
        for (Player* p : players_list) {
            if (!p->isActive() || p == &target)
                continue;

            auto* jud = dynamic_cast<Judge*>(p);
            if (!jud || jud->hasBlockedBribeThisTurn())
                continue;

            // Create popup prompt
            std::string question = "Judge " + jud->getName() + 
                ": Block bribe on " + target.getName() + "?";
            ReactionPopup popup(question);

            if (popup.ask()) {
                jud->PreventBribe();
                return true;
            }
        }
        return false;
    }


    bool Game::isGameOver() const {
    int activeCount = 0;
    for (Player* p : players_list)
        if (p->isActive()) ++activeCount;

    return activeCount == 1;
}

        


}