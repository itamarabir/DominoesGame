#include "Game.h"
#include <iostream>
using namespace std;

int main()
{
    char name[80];
    cout << "Enter your name:\n";
    cin >> name;
    
    Game* game = new Game(name);
    //דינאמי?
    game->runGame();
}