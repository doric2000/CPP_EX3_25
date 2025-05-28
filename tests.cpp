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

// Helper to extract names from game.players()
static std::vector<std::string> names(const std::vector<std::string>& v) { return v; }

TEST_CASE("Game basic functionality") {
    Game game;
    auto p1 = new Player(game, "Alice");
    auto p2 = new Player(game, "Bob");

    CHECK(game.turn() == "Alice");
    CHECK(names(game.players()) == std::vector<std::string>{"Alice","Bob"});
    CHECK_THROWS_AS(game.winner(), std::exception);
}

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

TEST_CASE("Bribe and extra turn") {
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

TEST_CASE("Arrest with Merchant special case success") {
    Game game;
    auto m = new Merchant(game, "M");
    auto p = new Player(game, "P");

    // Build M to 3 coins plus bonus
    for (int i = 0; i < 3; ++i) {
        m->gather(); // M's turn -> P
        p->gather(); // P's turn -> M (bonus applied)
    }
    CHECK(m->coins() == 4);

    // Now it's M's turn; switch to P for arrest
    game.nextTurn(); // M-> -> P
    CHECK(game.turn() == "P");
    p->arrest(*m);
    CHECK(m->coins() == 2);
    CHECK(game.turn() == "M");
}

TEST_CASE("Arrest with Merchant special case insufficient coins") {
    Game game;
    auto m = new Merchant(game, "M");
    auto p = new Player(game, "P");

    m->gather(); // M->1, turn->P
    CHECK(m->coins() == 1);

    // P's turn: arrest should throw
    CHECK_THROWS_AS(p->arrest(*m), std::runtime_error);
}

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

TEST_CASE("Sanction on Baron grants compensation and advances turn") {
    Game game;
    auto p = new Player(game, "P");
    auto bar = new Baron(game, "B");

    p->gather(); bar->gather();
    p->gather(); bar->gather();
    p->gather(); bar->gather();
    CHECK(bar->coins() == 3);

    p->sanction(*bar);
    CHECK(bar->coins() == 4);
    CHECK(game.turn() == "B");
}
