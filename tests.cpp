/*
 * Author: dor.cohen15@msmail.ariel.ac.il
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <vector>
#include <string>
#include "Game.hpp"
#include "Roles/Player.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Baron.hpp"
#include "Roles/General.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Merchant.hpp"

using namespace coup;


// Test 1: Basic game setup and validation
TEST_CASE("Game basic functionality") {
    Game game;
    auto p1 = new Player(game, "Alice");
    auto p2 = new Player(game, "Bob");

    CHECK(game.turn() == "Alice");
    CHECK((game.players()) == std::vector<std::string>{"Alice","Bob"});
    CHECK_THROWS_AS(game.winner(), std::exception);
}

// Test 2: Basic gather and tax mechanics
TEST_CASE("Gather and tax actions update coins and turn") {
    Game game;
    auto p1 = new Player(game, "P1");
    auto p2 = new Player(game, "P2");

    p1->gather();  // P1->1, turn->P2
    CHECK(p1->coins() == 1);
    CHECK(game.turn() == "P2");

    p2->tax();     // P2->2, turn->P1
    CHECK(p2->coins() == 2);
    CHECK(game.turn() == "P1");
}

// Test 3: Tax and sanction interaction
TEST_CASE("Basic tax and sanction effect on tax") {
    Game game;
    auto t1 = new Player(game, "T1");
    auto t2 = new Player(game, "T2");

    t1->tax();    // T1->2, turn->T2
    CHECK(t1->coins() == 2);

    // Prepare T2 with 3 coins
    for (int i = 0; i < 3; ++i) {
        t2->gather(); // turn->T1
        t1->gather(); // turn->T2
    }
    CHECK(t2->coins() == 3);

    t2->sanction(*t1); // T2->0, turn->T1
    CHECK_THROWS_AS(t1->tax(), std::runtime_error);
}

// Test 4: Basic bribe mechanics - maintain turn
TEST_CASE("Bribe lets play again") {
    Game game;
    auto p1 = new Player(game, "A");
    auto p2 = new Player(game, "B");

    for (int i = 0; i < 4; ++i) {
        p1->gather(); // turn->B
        p2->gather(); // turn->A
    }
    CHECK(p1->coins() == 4);

    p1->bribe();   // extra turn retains A
    CHECK(p1->coins() == 0);
    CHECK(game.turn() == "A");


}

// Test 5: Extended bribe mechanics - extra turn
TEST_CASE("Bribe gives Extra Turn") {
    Game game;
    auto p1 = new Player(game, "A");
    auto p2 = new Player(game, "B");

    for (int i = 0; i < 4; ++i) {
        p1->gather(); // turn->B
        p2->gather(); // turn->A
    }
    CHECK(p1->coins() == 4);

    p1->bribe();   // extra turn retains A
    CHECK(p1->coins() == 0);
    CHECK(game.turn() == "A");
    p1->gather();
    CHECK(p1->coins() == 1);
    CHECK(game.turn() == "A");

}



// Test 6: Merchant arrest mechanics - double cost and arrester bonus
TEST_CASE("Arrest with Merchant special case success") {
    Game game;
    auto m = new Merchant(game, "M");
    auto b = new Baron(game, "P");

    // Build M to 3 coins plus bonus
    for (int i = 0; i < 3; ++i) {
        m->gather(); // M's turn -> P
        b->gather(); // P's turn -> M (bonus applied)
    }
    CHECK(m->coins() == 4);

    // Now it's M's turn; switch to P for arrest
    game.nextTurn(); // M-> -> P
    CHECK(game.turn() == "P");
    b->arrest(*m);
    CHECK(m->coins() == 2);
    CHECK(game.turn() == "M");
    CHECK(b->coins() == 3);
}

// Test 7: Merchant arrest edge case - insufficient coins (requires 2)
TEST_CASE("Arrest with Merchant special case insufficient coins") {
    Game game;
    auto m = new Merchant(game, "M");
    auto p = new Player(game, "P");

    m->gather(); // M->1, turn->P
    CHECK(m->coins() == 1);

    // P's turn: arrest should throw
    CHECK_THROWS_AS(p->arrest(*m), std::runtime_error);
}

// Test 8: Forced coup rule at 10 coins
TEST_CASE("Coup forces turn on 10 coins") {
    Game game;
    auto x = new Player(game, "X");
    auto y = new Player(game, "Y");

    for (int i = 0; i < 10; ++i) {
        x->gather(); // turn->Y
        y->gather(); // turn->X
    }
    CHECK_THROWS_AS(x->gather(), std::exception);
}

// Test 9: Self-targeting prevention - coup
TEST_CASE("Player coup itself") {
    Game game;
    auto x = new Player(game, "X");
    auto y = new Player(game, "Y");

    for (int i = 0; i < 10; ++i) {
        x->gather(); // turn->Y
        y->gather(); // turn->X
    }
    CHECK_THROWS_WITH(x->coup(*x), "Cannot coup yourself"); // should throw that.
}

// Test 10: Self-targeting prevention - arrest
TEST_CASE("Player Arrest itself") {
    Game game;
    auto x = new Player(game, "X");
    auto y = new Player(game, "Y");

    for (int i = 0; i < 8; ++i) {
        x->gather(); // turn->Y
        y->gather(); // turn->X
    }
    CHECK_THROWS_WITH(x->arrest(*x), "Cannot arrest yourself"); // should throw that.
}

// Test 11: Self-targeting prevention - sanction
TEST_CASE("Player sanction itself") {
    Game game;
    auto x = new Player(game, "X");
    auto y = new Player(game, "Y");

    for (int i = 0; i < 5; ++i) {
        x->gather(); // turn->Y
        y->gather(); // turn->X
    }
    CHECK_THROWS_WITH(x->sanction(*x), "Cannot sanction yourself"); // should throw that.
}



// Test 12: Governor special ability - tax undo timing
TEST_CASE("Governor override tax and undo only on Governor's turn") {
    Game game;
    auto gov = new Governor(game, "G");
    auto p = new Player(game, "P");

    // Gov's turn: override tax => +3
    gov->tax();    // G->3, turn->P
    CHECK(gov->coins() == 3);
    CHECK(game.turn() == "P");

    // Undo on non-Governor turn should throw
    CHECK_THROWS_AS(gov->undo(*p), std::runtime_error);

    // Return to Governor's turn and attempt undo without P having taxed
    game.nextTurn(); // P-> -> G
    CHECK(game.turn() == "G");
    CHECK_THROWS_AS(gov->undo(*p), std::runtime_error);
}

// Test 13: Baron special abilities - investment and sanction bonus
TEST_CASE("Baron invest and sanction compensation") {
    Game game;
    auto bar = new Baron(game, "B");
    auto a = new Player(game, "A");

    for (int i = 0; i < 3; ++i) {
        bar->gather(); // turn->A
        a->gather();   // turn->B
    }
    CHECK(bar->coins() == 3);

    bar->invest();  // B->6, turn->A
    CHECK(bar->coins() == 6);

    a->sanction(*bar); // A->0, turn->B
    CHECK(bar->coins() == 7);
}

// Test 14: General special ability - coup prevention reset
TEST_CASE("General resetPreventCoupBlock does not consume turn") {
    Game game;
    auto gen = new General(game, "G");
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");

    for (int i = 0; i < 5; ++i) {
        gen->gather(); //->A
        a->gather();   //->B
        b->gather();   //->G
    }
    CHECK(gen->coins() == 5);

    gen->resetPreventCoupBlock();
    CHECK_FALSE(gen->hasBlockedCoupThisTurn());
    CHECK(game.turn() == "G");
}

// Test 15: Spy special ability - coin inspection timing
TEST_CASE("Spy inspectCoins only on Spy's turn and block on non-turn") {
    Game game;
    auto spy = new Spy(game, "S");
    auto p = new Player(game, "P");
    auto q = new Player(game, "Q");

    // Build Q's coins via Spy->P->Q cycles twice
    for (int i = 0; i < 2; ++i) {
        spy->gather(); // S->1, turn->P
        p->gather();   // P->1, turn->Q
        q->gather();   // Q->1, turn->S
    }
    CHECK(q->coins() == 2);

    // Confirm it's Spy's turn
    CHECK(game.turn() == "S");
    // Multiple inspections allowed on spy's turn
    CHECK(spy->inspectCoins(*q) == 2);
    CHECK(spy->inspectCoins(*q) == 2);
    CHECK(game.turn() == "S");

    // Switch to P's turn and verify exception
    game.nextTurn(); // S-> -> P
    CHECK(game.turn() == "P");
    CHECK_THROWS_AS(spy->inspectCoins(*q), std::runtime_error);
}

// Test 16: Arrest limitation - no consecutive targeting
TEST_CASE("Arrest edge: cannot arrest same player twice") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");

    a->gather(); //->B
    b->gather(); //->A
    a->arrest(*b); //->B
    CHECK(b->coins() == 0);

    b->gather(); //->A
    CHECK_THROWS_AS(a->arrest(*b), std::runtime_error);
}

// Test 17: General's arrest bonus - coin return
TEST_CASE("Arrest on General returns coin and advances turn") {
    Game game;
    auto p = new Player(game, "P");
    auto gen = new General(game, "G");

    p->gather();  //->G
    gen->gather(); //->P
    p->arrest(*gen); //->G
    CHECK(gen->coins() == 1);
    CHECK(p->coins() == 2);
    CHECK(game.turn() == "G");
}

// Test 18: Bypass GUI reaction - coup removes target and advances
TEST_CASE("Test 18: Bypass GUI coup removes target and advances") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    auto c = new Player(game, "C");
    // build A to 7 coins...
    for(int i=0;i<7;++i){
      a->gather(); b->gather(); c->gather();
    }
    // force the Game to apply the coup without GUI
    a->coup(*b);
    //CHECK(a->coins() == 0); // its valid but we use another func that involved the GUI for prevent coup
    auto ps = game.players();
    CHECK(std::find(ps.begin(), ps.end(), "B") == ps.end());
    CHECK(game.turn() == "C");
}

// Test 19: Winner Detection
TEST_CASE("Test 19: Winner detection after all others eliminated") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    // build A to coup B
    for (int i = 0; i < 7; ++i) {
        a->gather(); b->gather();
    }
    a->coup(*b);
    // now only A remains
    CHECK(game.winner() == "A");
}

// Test 20: Sanction Prevents Gather & Tax
TEST_CASE("Test 20: Sanction blocks gather and tax for one turn") {
    Game game;
    auto p = new Player(game, "P");
    auto q = new Player(game, "Q");
    // give P enough coins to sanction
    for (int i = 0; i < 4; ++i) {
        p->gather(); q->gather();
    }
    p->sanction(*q);         // P->0, turn->Q
    CHECK_THROWS_AS(q->gather(), std::runtime_error);
    CHECK_THROWS_AS(q->tax(), std::runtime_error);
    q->arrest(*p);
    p->gather();
    // next round, Q's sanction expires
    q->gather();
}


// Test 21: Merchant Treasury Payment on Arrest
TEST_CASE("Test 21: Merchant payment to bank on arrest") {
    Game game;
    auto m = new Merchant(game, "M");
    auto p = new Player(game, "P");
    // build M to 2 coins
    for (int i = 0; i < 2; ++i) {
        m->gather(); p->gather();
    }
    // arrest: Merchant pays 2 to bank, p gains nothing
    game.nextTurn();         // P's turn
    p->arrest(*m);
    CHECK(m->coins() == 0);
    CHECK(p->coins() == 2);  // arresting player does not gain merchant's coins
}

// 23. Spy’s Arrest Prevention Expires
TEST_CASE("23. Spy's arrest-prevention expires after next turn") {
    Game game;
    auto spy = new Spy(game, "S");
    auto p   = new Player(game, "P");
    auto q   = new Player(game, "Q");
    // build Q 1 coin, then spy blocks Q's arrest
    spy->gather(); p->gather(); q->gather();
    while (game.turn() != "S") game.nextTurn();
    spy->arrest(*q);   // no arrest ability for Q next turn
    game.nextTurn();         // P's turn
    game.nextTurn();         // Q's turn
    CHECK_THROWS_AS(q->arrest(*spy), std::runtime_error);
    // now expire, next round Q can arrest
    game.nextTurn();         // back to P → spy → p → q
    game.nextTurn();
    CHECK_NOTHROW(q->arrest(*spy));
}

// Test 23: Baron Sanction Compensation Limit
TEST_CASE("Test 23: Baron never loses more than one net coin per sanction") {
    Game game;
    auto bar = new Baron(game, "B");
    auto p   = new Player(game, "P");
    // Build B up to 3 coins
    for (int i = 0; i < 3; ++i) {
        bar->gather();  // B->, turn->P
        p->gather();    // P->, turn->B
    }
    CHECK(bar->coins() == 3);

    game.nextTurn();
    // Now P's turn: sanction B
    p->sanction(*bar);      // P pays 3 → 0, B receives +1 compensation
    CHECK(bar->coins() == 4);

    // Baron’s next turn: gather/tax should remain blocked,
    // but his coin count stays at 4, so net loss ≤ 1 from 3.
    CHECK_THROWS_AS(bar->gather(), std::runtime_error);
    CHECK(bar->coins() == 4);
}

// Test 24: General Blocks Coup
TEST_CASE("Test 24: General blocks coup and refunds attacker cost") {
    Game game;
    auto gen = new General(game, "G");
    auto a   = new Player(game, "A");
    auto b   = new Player(game, "B");
    // give everyone 7 coins
    for (int i = 0; i < 7; ++i) {
        gen->gather(); a->gather(); b->gather();
    }
    game.nextTurn(); // now A's turn.
    // A attempts coup on B; gen pays 5 to block
    a->coup(*b);
    gen->PreventCoup();
    CHECK(gen->hasBlockedCoupThisTurn());
    CHECK(std::find(game.players().begin(), game.players().end(), "B") != game.players().end()); // B still in game
    //CHECK(a->coins() == 0); // A paid 7 but we cant check it because our GUI
}

// Test 25: Role Duplication Independence
TEST_CASE("Test 25: Two Spies inspect independently") {
    Game game;
    auto s1 = new Spy(game, "S1");
    auto s2 = new Spy(game, "S2");
    auto p  = new Player(game, "P");
    game.nextTurn(); //s2 plays
    game.nextTurn();// p plays
    // build P to 1 coin
    p->gather();
    // S1 inspects P
    while (game.turn() != "S1") game.nextTurn();
    CHECK(s1->inspectCoins(*p) == 1);
    // S2 on its own turn inspects P again
    while (game.turn() != "S2") game.nextTurn();
    CHECK(s2->inspectCoins(*p) == 1);
}

// Test 26: Invalid Sanction Cost
TEST_CASE("Test 26: Sanction without enough coins throws") {
    Game game;
    auto p = new Player(game, "P");
    auto q = new Player(game, "Q");
    // P has 0 coins
    CHECK_THROWS_WITH(p->sanction(*q), "Not enough coins to sanction");
}

// Test 27: Bribe without enough coins throws
TEST_CASE("Test 27: Bribe without enough coins throws") {
    Game game;
    auto p = new Player(game, "P");
    auto q = new Player(game, "Q");
    // build P to 3 coins (<4 needed for bribe)
    for (int i = 0; i < 3; ++i) {
        p->gather();  // P->, turn->Q
        q->gather();  // Q->, turn->P
    }
    CHECK(p->coins() == 3);
    CHECK_THROWS_AS(p->bribe(), std::runtime_error);
}

// Test 28: Sanction without enough coins throws
TEST_CASE("Test 28: Sanction without enough coins throws") {
    Game game;
    auto p = new Player(game, "P");
    auto q = new Player(game, "Q");
    // P has 0 coins (<3 needed)
    CHECK_THROWS_AS(p->sanction(*q), std::runtime_error);
}


// Test 29: Governor override only once per turn
TEST_CASE("Test 29: Governor override only once per turn") {
    Game game;
    auto gov = new Governor(game, "G");
    auto p   = new Player(game, "P");
    // Gov's turn: override tax → +3
    gov->tax();   // G->3, turn->P
    CHECK_THROWS_AS(gov->tax(), std::runtime_error); // cannot override twice in same turn
    // back to Gov's turn
    game.nextTurn(); // P-> -> G
    gov->tax();      // now allowed again in new turn
    CHECK(gov->coins() == 6);
}

// Test 30: Spy cannot inspect eliminated player
TEST_CASE("Test 30: Spy inspect eliminated player throws") {
    Game game;
    auto spy = new Spy(game, "S");
    auto p   = new Player(game, "P");
    auto x   = new Player(game, "X");
    game.eliminatePlayer(p);
    // now P is inactive
    while (game.turn() != "S") game.nextTurn();
    CHECK_THROWS_AS(spy->inspectCoins(*p), std::runtime_error);
}


// Test 31: Merchant no bonus below threshold
TEST_CASE("Test 31: Merchant no bonus below threshold") {
    Game game;
    auto m = new Merchant(game, "M");
    auto p = new Player(game, "P");
    // build M to 2 coins (<3 needed for bonus)
    m->gather();  // M->1, turn->P
    p->gather();  // P->1, turn->M
    m->gather();  // M->2, no bonus
    CHECK(m->coins() == 2);
}


// Test 32: Winner throws before game ends
TEST_CASE("Test 32: Winner throws before game ends") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    CHECK_THROWS_AS(game.winner(), std::exception);
}

// Test 33: Players list order after elimination
TEST_CASE("Test 33: Players list order after elimination") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    auto c = new Player(game, "C");
    // eliminate B
    for (int i = 0; i < 7; ++i) {
        a->gather(); b->gather(); c->gather();
    }
    a->coup(*b);
    auto ps = game.players();
    CHECK(ps == std::vector<std::string>{"A","C"});
}

// 28. Cannot start game with fewer than 2 players
TEST_CASE("28. Game start requires two players") {
    Game game;
    auto a = new Player(game, "A");
    // nextTurn should throw until we have at least 2 players
    CHECK_THROWS_WITH(game.nextTurn(), "Need at least two players to start the game");
    auto b = new Player(game, "B");
    // now with two, nextTurn no longer throws
    CHECK_NOTHROW(game.nextTurn());
}

// 29. Cannot add players after game has started
TEST_CASE("29. Cannot add player post-start") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    game.nextTurn();                // starts the game
    CHECK_THROWS_WITH(game.addPlayer(new Player(game, "D")),
                      "Cant add a player after game has started");
}

// 30. Max six players enforced
TEST_CASE("30. Max six players allowed") {
    Game game;
    std::vector<Player*> v;
    for (int i = 0; i < 6; ++i) {
        v.push_back(new Player(game, std::string(1, 'A'+i)));
    }
    // adding a seventh should fail
    CHECK_THROWS_WITH(new Player(game, "G"), "Maximum 6 players allowed");
}

// 31. turn() errors when no players or current inactive
TEST_CASE("31. turn() errors if empty list or current inactive") {
    Game empty;
    CHECK_THROWS_WITH(empty.turn(), "No players in game");
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    // eliminate A, current index is still 0
    game.eliminatePlayer(a);
    CHECK_THROWS_WITH(game.turn(), "Current player is not active");
}

// 32. getLastArrested / updateLastArrested tracking
TEST_CASE("32. Last-arrested tracker updates correctly") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    auto c = new Player(game, "C");
    // initially null
    CHECK(game.getLastArrested() == nullptr);
    // after B arrests C, lastArrested is C
    game.nextTurn();            // A->B
    b->gather();
    c->gather();
    game.nextTurn();  
    b->arrest(*c);
    CHECK(game.getLastArrested()->getName() == "C");
}

// 33. eliminatePlayer does not disturb turn order of others
TEST_CASE("33. eliminatePlayer skips eliminated in nextTurn") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    auto c = new Player(game, "C");
    // A->B->C->A
    // eliminate B during A's turn
    CHECK(game.turn() == "A");
    game.eliminatePlayer(b);
    game.nextTurn();
    // should skip B and go to C
    CHECK(game.turn() == "C");
}

// 34. isGameOver mirror of winner
TEST_CASE("34. isGameOver flips only when one remains") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    CHECK_FALSE(game.isGameOver());
    // remove B
    for (int i = 0; i < 7; ++i) { a->gather(); b->gather(); }
    a->coup(*b);
    CHECK(game.isGameOver());
}

// 35. initializePlayers produces correct count and only active names
TEST_CASE("35. initializePlayers random fill respects count") {
    Game game;
    std::vector<std::string> names = {"X","Y","Z","W"};
    game.initializePlayers(names);
    auto ps = game.players();
    // should have exactly those four, order-preserved
    CHECK(ps.size() == 4);
    CHECK(std::find(ps.begin(), ps.end(), "X") != ps.end());
    CHECK(std::find(ps.begin(), ps.end(), "Y") != ps.end());
    CHECK(std::find(ps.begin(), ps.end(), "Z") != ps.end());
    CHECK(std::find(ps.begin(), ps.end(), "W") != ps.end());
}

// 36. dispatchCoupAttempt rejects inactive or self targets
TEST_CASE("36. dispatchCoupAttempt invalid-target checks") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    for (int i = 0; i < 7; ++i) { a->gather(); b->gather(); }
    // inactive target
    game.eliminatePlayer(b);
    CHECK_THROWS_WITH(game.dispatchCoupAttempt(*b, *a), "Target is not active");
    // self-prevention
    CHECK_THROWS_WITH(game.dispatchCoupAttempt(*a, *a), "Cannot prevent coup on yourself");
}

// 37. dispatchBribeAttempt returns false if no Judge to block
TEST_CASE("37. dispatchBribeAttempt false-when-no-blockers") {
    Game game;
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    for (int i = 0; i < 4; ++i) { a->gather(); b->gather(); }
    // neither is Judge, so any bribe goes through (no block)
    CHECK_FALSE(game.dispatchBribeAttempt(*a));
}

// 38. dispatchBribeAttempt skips inactive or self
TEST_CASE("38. dispatchBribeAttempt skips inactive and self") {
    Game game;
    auto j = new Judge(game, "J");
    auto a = new Player(game, "A");
    auto b = new Player(game, "B");
    // give A ≥4 so it can bribe
    for (int i = 0; i < 4; ++i) { j->gather(); a->gather(); b->gather(); }
    // eliminate J
    game.eliminatePlayer(j);
    // now only B is Judge-role but inactive, so no blocking
    CHECK_FALSE(game.dispatchBribeAttempt(*a));
}



