#ifndef PLAYER_H
#define PLAYER_H
#include "Pile.h"
#include "Stone.h"
#pragma warning( disable: 4996)


class Player
{
private:
    const int STARTING_PILE_SIZE = 7;
    char* name;
    int pileSize; // Defaults at 0
    Pile* playerPile; // Ptr to a Pile object
    bool isHuman; // Human = true, Comp = false

public:
    // C/D:
    Player(); // Default Construcor
    Player(const Player&); // Copy constructor
    Player(const char*, bool); // Main Constructor (@name, @isHuman)
    ~Player(); // Destructor

    // Getters:
    const int getPileSize() { return this->pileSize; };
    char* getPlayerName();
    Pile& getPlayerPile();


    // Setters:
    void setName(const char*);

    // Actions:
    Stone removeStone(const int); // Removes stone by index.
    void addStone(Stone); // Adds a Stone to (@playerPile)


    // Printer:
    void printPile();

    // Misc:
    Player operator=(Player pl);




};

#endif