#ifndef PILE_H
#define PILE_H
#include "Stone.h"
#pragma warning( disable: 4996)

class Pile
{
private:
    Stone* stones_array; // stones in the pile
    int pileSize; // corollates exactly to stones_array length
    const int MAX_PILE_SIZE = 28;


public:
    // C/D:
    Pile();     // Default Constructor
    Pile(const Pile&); // Copy Constructor
    ~Pile() { delete[] stones_array; } // Destructor

    // Actions:
    void addStone(Stone, Stone::side);
    void fillPile();
    void shufflePile();
    Stone removeStone(int);

    // Getters:
    const int getPileSize() { return this->pileSize; }
    Stone stoneByIndex(int); // Return a stone by index
    Stone* getStonesArray();
    Stone* getStoneArrayPtr();

    // Printers:
    const void printOpen();
    const void printClosed();

    // Misc:
    bool isEmpty();
    Pile operator=(Pile);
};

#endif