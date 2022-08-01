#include "Stone.h"
#include <iostream>
using namespace std;

// Constructor:
Stone::Stone(int left, int right) {
    setStone(left, right);
}


// Setters:

void Stone::setStone(int left, int right) {
    this->left = left;
    this->right = right;
}

// Actions:
void Stone::rotateStone() {
    int temp = right;
    right = left;
    left = temp;
}

// Printers:
const void Stone::printOpen() {
    cout << "[" << getLeft()
        << "][" << getRight() << "]";

}

const void Stone::printClosed() {
    cout << "[ ][ ]";
}

// Misc:

// Returns true if both stones are equal on BOTH sides.
//אופרטור ==?
const bool Stone::compareStones(Stone s2) {
    return  (right == s2.getRight() && left == s2.getLeft()) ||
        (left == s2.getRight() && right == s2.getLeft());
}