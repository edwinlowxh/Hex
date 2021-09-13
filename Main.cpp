#include "HexBoard.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <windows.h>
using namespace std;

int main() {
  int size, i, j;
  char player_choice;
  player p;
  player c;

  //Get the board size
  cout << "Size of Hex board: ";
  cin >> size;
  HexBoard hexBoard(size);

  //Get the human player choice
  cout << "Choose player(R/B): " << endl;
  cout << "R(North-South) / B(East-West)" << endl;
  cin >> player_choice;

  if (player_choice == 'R'){
    c = B;
    p = R;
  }
  else{
    c = R;
    p = B;
  }

  //Random starting player
  srand(time(0));
  if (false){
    cout << "Computer starts." << endl;
    hexBoard.computer(c);
    hexBoard.printHexBoard();
  }
  else{
    cout << "You start." << endl;
  }

  bool won = false;
  while(!won){
    cout << "Move(i , j): ";
    cin >> i >> j;

    //Check if player move is legal
    while (!hexBoard.isLegal(i, j)){
      cout << "Move(i , j): ";
      cin >> i >> j;
    }

    hexBoard.playerMove(p, i, j);
    hexBoard.printHexBoard();
    if (hexBoard.win(p)){
      cout << "You Won" << endl;
      break;
    }
    if (hexBoard.computer(c) == c){
      hexBoard.printHexBoard();
      cout << "Computer Won" << endl;
      break;
    }
    hexBoard.printHexBoard();
  }
  return 0;
}
