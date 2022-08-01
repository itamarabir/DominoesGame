#ifndef STONE_H
#define STONE_H
#pragma warning( disable: 4996)

class Stone {
private:
    int right, left;

public:
    // Constructors:
    Stone() { right = -1; left = -1; }; // Default Constructor
    Stone(int, int); // The standart constructor (@Left,@Right)

    // Getters:
    int getRight() { return right; }
    int getLeft() { return left; }

    // Setters:
    void setStone(int, int);

    // Actions:
    void rotateStone();

    // Printers:
    const void printOpen();
    const void printClosed();

    // Misc:
    const bool compareStones(Stone s2);
    enum side { Left = 0, Right };
};
#endif