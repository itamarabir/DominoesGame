#include "Player.h"
#include "Pile.h"
#include <iostream>
#include <cassert>
#include <string.h>
using namespace std;
// C/D:
// Default Constructor, sets empty values to fields
Player::Player() {
    name = new char[1];
    playerPile = new Pile();
    pileSize = 0;
    isHuman = true;
}

// Copy Constructor
// Takes data from const reference @player, Initializes 'this' as a copy of @player.
Player::Player(const Player& player) {
    // copy name
    name = new char[strlen(player.name) + 1];
    assert(name != 0);
    strcpy(name, player.name);
    // copy pile size
    pileSize = player.pileSize;
    // copy pile
    playerPile = new Pile(*player.playerPile);
    // copy isHuman
    isHuman = player.isHuman;
}

// Main Constructor. Initializes @name and @isHuman, and pile to empty
Player::Player(const char* name, bool isHuman) {
    this->isHuman = isHuman;
    setName(name);
    playerPile = new Pile();
    pileSize = 0;
}

// Destructor
Player::~Player() {
    if (name != 0)
        delete[] name;
    if (playerPile != 0)
        delete(playerPile);
}

// Getters:
Pile& Player::getPlayerPile() {
    //Pile newPile(*playerPile);
    return *playerPile;
}

char* Player::getPlayerName() {
    char* s = new char[strlen(name)];
    strcpy(s, name);
    return s;
}

// Setters:
void Player::setName(const char* name) {
    this->name = new char[strlen(name)];
    assert(this->name != 0);
    strcpy(this->name, name);
}

// Actions:
// Removes stone from playerPile by @index
Stone Player::removeStone(const int index) {
    Stone removedStone = playerPile->removeStone(index);
    pileSize--;
    return removedStone;
}

// Adds @stoneToAdd to playerPile
void Player::addStone(Stone stoneToAdd) {
    playerPile->addStone(stoneToAdd, Stone::Right);
    pileSize++;
}

// Printer:
void Player::printPile()
{
    if (isHuman)
        playerPile->printOpen();
    else
        playerPile->printClosed();
}


// Misc:
Player Player::operator=(Player pl) {
    if (name != 0)
        delete[] name;
    name = new char[strlen(pl.name) + 1];
    assert(name != 0);
    strcpy(name, pl.name);
    pileSize = pl.pileSize;
    playerPile = pl.playerPile;
    isHuman = pl.isHuman;
    return *this;
}