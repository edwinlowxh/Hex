#include "Cell.h"

using namespace std;

Cell::Cell(int i, int j, vector<int> neighbors, player p, player simulated): i(i), j(j), neighbors(neighbors), p(p), simulated(simulated){};

void Cell::setPlayer(player n){
  p = n;
}
player Cell::getPlayer(){
  return p;
}
void Cell::setSimulated(player n){
  simulated = n;
}
player Cell::getSimulated(){
  return simulated;
}
vector<int> Cell::getNeighbors(){
  return neighbors;
}
int Cell::J(){
  return j;
}
int Cell::I(){
  return i;
}
