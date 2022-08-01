#include "Pile.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

// C/D:

// Main constructor. Initializes a Human and Computer players
Game::Game(const char* humanName)
{
    char* n = new char[strlen(humanName)];
    strcpy(n, humanName);
    humanPlayer = new Player(n, true);

    computerPlayer = new Player("COMPUTER", false);

    bankPile = new Pile();
    bankPile->fillPile();
    bankPile->shufflePile();

    tablePile = new Pile();

    startUpPlayerHand(*humanPlayer);
    startUpPlayerHand(*computerPlayer);
    leftEdge = rightEdge = -1;
}

Game::~Game()
{
    if (tablePile != 0)
        delete(tablePile);
    if (bankPile != 0)
        delete(bankPile);
    if (humanPlayer != 0)
        delete(humanPlayer);
    if (computerPlayer != 0)
        delete(computerPlayer);
}

// Printer:
void Game::printBoard()
{
    cout << "B A N K:\nstart-->";
    bankPile->printClosed();
    cout << "<--end\n";

    // Computer Side:
    cout << "****** " << computerPlayer->getPlayerName() << " ******\n";
    computerPlayer->getPlayerPile().printClosed();
    cout << "\n";
    for (int i = 0; i < computerPlayer->getPlayerPile().getPileSize(); i++)
        cout << " - " << i + 1 << " -  ";


    // Main Board:
    cout << "\n\n\n";
    tablePile->printOpen();
    cout << "\n\n\n";

    // Player Side:
    cout << "****** " << humanPlayer->getPlayerName() << " ******\n";
    humanPlayer->getPlayerPile().printOpen();
    cout << "\n";
    for (int i = 0; i < humanPlayer->getPlayerPile().getPileSize(); i++)
        cout << " - " << i + 1 << " -  ";

}

// Game Manager:

void Game::runGame()
{
    // 1.) check whose turn it is
    // first turn is whoever has a 6-6 and else its the player
    Stone sixStone(6, 6);
    const int PLAYER = 1, COMP = 2;
    int nextPlayer = PLAYER;         // default: player goes first
    for (int i = 0; i < computerPlayer->getPileSize(); i++) {
        if (sixStone.compareStones(computerPlayer->getPlayerPile().getStonesArray()[i]))
            nextPlayer = COMP;      // Computer goes first
    }

    cout << "\n*****GAME STARTS******\n\n";
    printBoard();
    // Print starting message:
    if (nextPlayer == PLAYER)
        cout << "\nYou start! Press Enter";
    else
        cout << "\nComputer Starts! Press Enter";

    cin.ignore();

    system("CLS");
    // Game Loop:
    while (true) {
        if (isOver())
        {

            break;
        }
        if (nextPlayer == PLAYER) {
            humanTurn();
            nextPlayer = COMP;
            system("CLS");
        }
        else
        {
            computerTurn();
            nextPlayer = PLAYER;
            cout << "@@ End of Computers turn.\n";
        }

    }

    // Calculate points, declare winner
    endGame();
}


// Private helper methods:

void Game::endGame() {
    printBoard();
    cout << "\n\n\n\n***********************************************************\n\n\t\t\tEND GAME\n\n***********************************************************\n";

    if (humanPlayer->getPileSize() < 1)
    {
        cout << "Congratulations, " << humanPlayer->getPlayerName() << "! You have won!\n";
        return;
    }
    else if (computerPlayer->getPileSize() < 1)
    {
        cout << "Tough Luck, " << humanPlayer->getPlayerName() << "! You have Lost!    Computers victory!\n";
        return;
    }
    else
    {
        int playerScore = 0;
        int computerScore = 0;
        cout << "\nHUMAN PILE : " << humanPlayer->getPileSize() << "COMP PILE: " << computerPlayer->getPileSize() << "\n\n";
        for (int i = 0; i < humanPlayer->getPileSize(); i++)
            playerScore += humanPlayer->getPlayerPile().getStonesArray()[i].getLeft() + humanPlayer->getPlayerPile().getStonesArray()[i].getRight();
        for (int i = 0; i < humanPlayer->getPileSize(); i++)
            computerScore += computerPlayer->getPlayerPile().getStonesArray()[i].getLeft() + computerPlayer->getPlayerPile().getStonesArray()[i].getRight();

        cout << "\nComputers Stones:\n";
        computerPlayer->printPile();
        cout << "\nYour Stones:\n";
        humanPlayer->printPile();

        cout << "\nYour Score: " << playerScore << "\tComputers Score: " << computerScore << "\n";
        if (playerScore > computerScore) {
            cout << "\nCongratulations, " << humanPlayer->getPlayerName() << "! You have won!\n";
            return;
        }
        else if (playerScore < computerScore) {
            cout << "\nTough Luck, " << humanPlayer->getPlayerName() << "! You have Lost!    Computers victory!\n";
            return;
        }
        else {          // ==
            cout << "\nIt's a Tie! Better luck next time!\n";
            return;
        }

    }
}

// true if player @p has any legal moves, else false
bool Game::hasLegalMove(Player p) {
    for (int i = 0; i < p.getPileSize(); i++)
        if (isStoneLegal(p.getPlayerPile().getStonesArray()[i]))
            return true;
    return false;
}

// true if a Stone @stone can be placed on the table, else false
bool Game::isStoneLegal(Stone stone) {
    if (tablePile->getPileSize() == 0)
        return true;
    if (stone.getLeft() == leftEdge ||
        stone.getRight() == leftEdge ||
        stone.getLeft() == rightEdge ||
        stone.getRight() == rightEdge)
        return true;
    return false;
}

// Method to fill a players hand with 7 cards from the bankPile, must be run at the beginning of a game
void Game::startUpPlayerHand(Player& p) {

    for (int i = 0; i < 7; i++)
        p.addStone(bankPile->removeStone(0));
}

// returns true if any player has an empty hand, or both players have no moves and the bank is empty
// else false
bool Game::isOver() {
    if (humanPlayer->getPlayerPile().isEmpty() ||
        computerPlayer->getPlayerPile().isEmpty() ||
        (!hasLegalMove(*humanPlayer) && !hasLegalMove(*computerPlayer) && bankPile->isEmpty()))
        return true;
    return false;
}

void Game::computerTurn() {
    cout << "\nIt's the computers turn.\n";
    // No legal moves -> either grab from the bank or pass the turn.
    bool hasLegal = hasLegalMove(*computerPlayer);
    bool bankEmpty = bankPile->isEmpty();
    if (!hasLegal && !bankEmpty) {
        computerPlayer->addStone(bankPile->removeStone(0));
        cout << "\nComputer picked up a card from the bank\n";
        return;
    }
    else if (!hasLegal && bankEmpty) {
        cout << "Computer player has no move.\n";
        return;
    }

    // Find the best move:

    // search for double
    for (int i = 0; i < computerPlayer->getPileSize(); i++) {
        Stone currentStone = computerPlayer->getPlayerPile().getStonesArray()[i];
        if (currentStone.getLeft() == currentStone.getRight() &&
            isStoneLegal(currentStone))
        {
            // A double has been found and its corollating to one of the sides.

            // Note: if table not empty, no need to change edges because adding double doesnt change a thing!
            if (currentStone.getLeft() == leftEdge)
                tablePile->addStone(computerPlayer->removeStone(i), Stone::Left);
            else
                tablePile->addStone(computerPlayer->removeStone(i), Stone::Right);

            // if table was empty (if this is the first stone):
            if (tablePile->getPileSize() == 1)
                leftEdge = rightEdge = currentStone.getLeft();

            cout << "Computer played: ";
            currentStone.printOpen();
            cout << "   Computers turn ends.\n";
            return;
        }
    }

    int highestSum = 0;
    int highestIndex = 0;
    Stone currentStone;
    // search for highest sum
    for (int i = 0; i < computerPlayer->getPileSize(); i++) {
        currentStone = computerPlayer->getPlayerPile().getStonesArray()[i];
        // if current stone CAN'T be played, check the next stone
        if (!isStoneLegal(currentStone))
            continue;
        int currentSum = currentStone.getLeft() + currentStone.getRight();
        if (currentSum > highestSum)
        {
            highestSum = currentSum;
            highestIndex = i;
        }
    }

    currentStone = computerPlayer->getPlayerPile().getStonesArray()[highestIndex];
    // play the highest sum stone
    // check for rotation:
    if (currentStone.getLeft() == leftEdge || currentStone.getRight() == rightEdge)
        computerPlayer->getPlayerPile().getStoneArrayPtr()[highestIndex].rotateStone();
    // add to correct side:
    currentStone = computerPlayer->getPlayerPile().getStonesArray()[highestIndex]; // get the rotated stone this time

    // add to left
    if (currentStone.getRight() == leftEdge) {
        leftEdge = computerPlayer->getPlayerPile().getStonesArray()[highestIndex].getLeft();
        tablePile->addStone(computerPlayer->removeStone(highestIndex), Stone::Left);
    }
    else    // add to right
    {
        rightEdge = computerPlayer->getPlayerPile().getStonesArray()[highestIndex].getRight();
        tablePile->addStone(computerPlayer->removeStone(highestIndex), Stone::Right);
    }
    cout << "Computer played: ";
    currentStone.printOpen();
    cout << "   Computers turn ends.\n";
}


void Game::humanTurn() {

    cout << "\nIt's your turn, \n" << humanPlayer->getPlayerName() << "!\n";
    cout << "This is your current board:\n";
    printBoard();
    // No legal moves -> grab from bank if possible and pass turn.
    bool hasLegal = hasLegalMove(*humanPlayer);
    bool bankEmpty = bankPile->isEmpty();
    if (!hasLegal && !bankEmpty) {
        cout << "\nYou have no legal moves. A stone will be added to your hand.\nPress \"enter\" to pass:";
        cin.ignore();
        humanPlayer->addStone(bankPile->removeStone(0));
        return;
    }
    else if (!hasLegal && bankEmpty) {
        cout << "\nYou have no legal moves. The bank pile is empty.\nPress \"enter\" to pass:";
        cin.ignore();
        return;
    }

    // Play turn
    int stoneIndex;
    while (true) {
        cout << "\nPlease pick the index of a stone to place on the board: ";
        cin >> stoneIndex;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (stoneIndex < 1 ||           // Wrong index
            stoneIndex > humanPlayer->getPileSize() ||
            !isStoneLegal(humanPlayer->getPlayerPile().getStonesArray()[stoneIndex - 1])) // Stone is illegal
        {
            cout << "\nOops! Wrong stone index! Try Again.";
            continue;
        }
        break;
    }
    stoneIndex--; // Board starts at 1 while index starts at 0


    int stoneLeft = humanPlayer->getPlayerPile().getStonesArray()[stoneIndex].getLeft();
    int stoneRight = humanPlayer->getPlayerPile().getStonesArray()[stoneIndex].getRight();
    // If the table is empty, just place the stone and end.
    if (tablePile->getPileSize() == 0) {
        tablePile->addStone(humanPlayer->removeStone(stoneIndex), Stone::Right);
        leftEdge = stoneLeft;
        rightEdge = stoneRight;
        return;
    }

    // If the stone can be played on both sides
    if ((stoneLeft == leftEdge && stoneLeft == rightEdge) ||
        (stoneLeft == leftEdge && stoneRight == rightEdge) ||
        (stoneRight == leftEdge && stoneRight == rightEdge) ||
        (stoneRight == leftEdge && stoneLeft == rightEdge))
    {
        int c; // Choice of side
        while (true) {
            // Ask the player which side he wants to play.
            cout << "\nThe stone you have chosen can be played on both sides. Which side would you like?\n";
            cout << "1 - left   <--  -->   2 - right\n";

            // Ask again ?
            cin >> c;
            if (c != 1 && c != 2) {
                cout << "Oops. Input 1 or 2 only! Try Again.";
                continue;
            }
            break;
        }
        switch (c) {
        case 1: // Rotate & Place on Left
            if (stoneLeft != stoneRight && stoneLeft == leftEdge)
                humanPlayer->getPlayerPile().getStoneArrayPtr()[stoneIndex].rotateStone();
            tablePile->addStone(humanPlayer->removeStone(stoneIndex), Stone::Left);
            leftEdge = tablePile->getStonesArray()[0].getLeft();

            break;
        case 2: // Rotate & Place on Right
            if (stoneLeft != stoneRight && stoneRight == rightEdge)
                humanPlayer->getPlayerPile().getStoneArrayPtr()[stoneIndex].rotateStone();
            tablePile->addStone(humanPlayer->removeStone(stoneIndex), Stone::Right);
            rightEdge = tablePile->getStonesArray()[tablePile->getPileSize() - 1].getRight();

            break;
        }
    }
    else {
        // Rotate stone if needed
        if (stoneLeft == leftEdge || stoneRight == rightEdge)
        {
            humanPlayer->getPlayerPile().getStoneArrayPtr()[stoneIndex].rotateStone();
            int temp = stoneLeft;
            stoneLeft = stoneRight;
            stoneRight = temp;
        }
        // Play on the correct side
        if (stoneRight == leftEdge) {
            tablePile->addStone(humanPlayer->removeStone(stoneIndex), Stone::Left);
            leftEdge = tablePile->getStonesArray()[0].getLeft();
        }
        else {
            tablePile->addStone(humanPlayer->removeStone(stoneIndex), Stone::Right);
            rightEdge = tablePile->getStonesArray()[tablePile->getPileSize() - 1].getRight();
        }
    }
}