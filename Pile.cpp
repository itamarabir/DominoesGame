#include "Pile.h"
#include "Stone.h"
#include <cassert>
#include <iostream>
#include <time.h>
using namespace std;

// C/D:
// Default constructor. Sets empty values to fields.
Pile::Pile() {

    stones_array = new Stone[1];//למה לא 0
    pileSize = 0;
}

// Copy Constructor
// Takes the data from the const reference @pile, Initializes 'this' as copy of @pile.
Pile::Pile(const Pile& pile) {
    stones_array = new Stone[pile.pileSize]; // Equal the lengths
    for (int i = 0; i < pile.pileSize; i++)
        stones_array[i] = Stone(pile.stones_array[i].getLeft(), pile.stones_array[i].getRight());
    pileSize = pile.pileSize;
}


// Actions:
// Adds a @stoneToAdd Stone into the stones_array, if its possible
void Pile::addStone(Stone stoneToAdd, Stone::side side) {
    // Check that stones_array isn't full
    if (pileSize == MAX_PILE_SIZE) {
        cout << "Can't add additional stones!" << endl;
        exit(0);
    }
    pileSize++;         // Incrementing the pile size off the get go
    //dynamic arrays
    Stone* newStoneArr = new Stone[pileSize];   // To be set as new main array
    assert(newStoneArr != 0);
    int i = 0;
    if (side == Stone::Left) {
        // Add to left side:
        newStoneArr[0] = stoneToAdd; // First add the stone
        i++;
        for (; i < pileSize; i++) // Then copy rest of array
            newStoneArr[i] = Stone(stones_array[i - 1].getLeft(), stones_array[i - 1].getRight());
    }
    else {
        // Add to rigth side:
        for (; i < pileSize - 1; i++) // First copy array
            newStoneArr[i] = Stone(stones_array[i].getLeft(), stones_array[i].getRight());
        newStoneArr[i] = stoneToAdd;  // Then add the stone
    }
    delete[] stones_array;//שחרור הזיכרון
    stones_array = newStoneArr;// יחזיק כתובת חדשה
    newStoneArr = NULL;
}

// fills the pile with all the relevant stones. 0-0, 1-0, 1-1, .... 6-5, 6-6.
void Pile::fillPile() {
    const int MAX_SIZE = 6;
    for (int i = 0; i <= MAX_SIZE; i++)
        for (int j = 0; j <= i; j++)
            addStone(Stone(i, j), Stone::Right);
}

// reshufles the order of the stones in stones_array
void Pile::shufflePile() {
    srand(time(0));
    int randIndex;
    Stone* newStoneArr = new Stone[pileSize];
    for (int i = 0; i < pileSize; i++) {
        // 1. Pick random index
        randIndex = rand() % MAX_PILE_SIZE;
        // 2. if theres a Stone there add it to the next place in a new array
        if (stones_array[randIndex].getLeft() != -1) {
            newStoneArr[i] = Stone(stones_array[randIndex].getLeft(), stones_array[randIndex].getRight());//אופרטור השמה?
            stones_array[randIndex].setStone(-1, -1);// change stone to -1, -1
        }
        else {
            // 3. if there isnt a number there, search to the right then search to the left until you find a number
            // Look Right:
            int j = randIndex + 1;
            while (j < pileSize && stones_array[j].getLeft() == -1)
                j++;
            if (j == pileSize) {
                // Look Left:
                j = randIndex - 1;
                while (j > 0 && stones_array[j].getLeft() == -1)
                    j--;
            }
            newStoneArr[i] = Stone(stones_array[j].getLeft(), stones_array[j].getRight());
            stones_array[j].setStone(-1, -1);
        }
    }
    delete[] stones_array;
    stones_array = newStoneArr;
    newStoneArr = NULL;
}

Stone Pile::removeStone(int index) {
    if (pileSize == 0 || index < 0 || index > pileSize) {
        cout << "Illegal removal request: " << index << "\n";
        exit(0);
    }
    Stone removed = Stone(stones_array[index].getLeft(), stones_array[index].getRight());
    Stone* newStoneArr = new Stone[pileSize - 1];
    // למה כל פעם עושים לו הקצאה חדשה? השתמשנו בו כבר ב2 פונקציות
    //הקצאה שאנחנו עושים וכל פעם משחררים אותה רק כדי חעבוד איתה?
    assert(newStoneArr != 0);
    int j = 0;
    for (int i = 0; i < pileSize - 1; i++, j++) {
        if (i == index)
            j++;
        newStoneArr[i] = Stone(stones_array[j].getLeft(), stones_array[j].getRight());
    }
    pileSize--;
    delete[] stones_array;
    stones_array = newStoneArr;
    newStoneArr = NULL;
    return removed;
}
// Getters:
Stone Pile::stoneByIndex(int index) {
    if (index > pileSize || index < 0) {
        cout << "Wrong index!" << endl;
        exit(0);
    }
    //return Stone(stones_array[index].getLeft(), stones_array[index].getRight());
    return stones_array[index];
}

Stone* Pile::getStonesArray() {
    Stone* newStonesArr = new Stone[pileSize];
    for (int i = 0; i < pileSize; i++)
        newStonesArr[i] = Stone(stones_array[i].getLeft(), stones_array[i].getRight());
    return newStonesArr;//החזרת מצביע
}

Stone* Pile::getStoneArrayPtr() {
    return stones_array;
}

// Printers:
const void Pile::printOpen() {
    for (int i = 0; i < pileSize; i++) {
        stones_array[i].printOpen();
        cout << ", ";
    }

}

const void Pile::printClosed() {
    for (int i = 0; i < pileSize; i++) {
        stones_array[i].printClosed();
        cout << ", ";
    }
}

// Misc:


Pile Pile::operator=(Pile  p) {
    delete[] stones_array;
    pileSize = p.getPileSize();
    for (int i = 0; i < pileSize; i++)
        stones_array[i] = Stone(p.getStonesArray()[i].getLeft(), p.getStonesArray()[i].getRight());
    return *this;
}

bool Pile::isEmpty() {
    return pileSize == 0;
}