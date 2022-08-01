#ifndef GAME_H
#define GAME_H
#include "Player.h"
#pragma warning( disable: 4996)
class Game {
private:
    Player* humanPlayer;    // Ptr to the human player
    Player* computerPlayer; // Ptr to the comp player
    Pile* bankPile;         // Ptr to the bank
    Pile* tablePile;        // Ptr to the pile on the table
    int leftEdge, rightEdge;// The left and right edges of the tablePile


    //  Private helper methods:

    bool isOver();  // Check whether a victory or loss condition has been achieved
    bool hasLegalMove(Player); // Check if a player has legal moves
    bool isStoneLegal(Stone); // check if a particular stone is legal
    void startUpPlayerHand(Player&); // Start up a hand with 7 cards

    void computerTurn(); // manage the computers turn logic

    void humanTurn();   // handle the human turn i/o, legality of moves, etc..
    void endGame();
public:
    // C/D:
    Game(const char*);
    ~Game();

    // Printer:
    void printBoard();

    // Game Manager:
    void runGame();

};

#endif