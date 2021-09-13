#include "Cell.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <algorithm>
#include <chrono>
#include <cstdlib>
using namespace std;

class HexBoard{
public:
  void initialiseBoard(int size);
  HexBoard(int size);
  HexBoard(const HexBoard &c);
  void playerMove(player p, int i, int j);
  void undoMove(int i, int j);
  void simulatedMove(player p, int i, int j);
  void resetSimulation();
  bool isLegal(int i, int j);
  bool win(player p);
  player simulate(HexBoard &h, player p, vector<tuple<int, int>> &legalMoves);
  player computer(player c);
  void printHexBoard();
  int getSize();
  std::vector<Cell> getCells();

private:
  std::vector<Cell> Cells;
  int size;
};
