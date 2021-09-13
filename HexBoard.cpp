#include "HexBoard.h"

HexBoard::HexBoard(int size): Cells(size * size), size(size){initialiseBoard(size);}

HexBoard::HexBoard(const HexBoard &c){ //Copy constructor
  Cells = c.Cells;
  size = c.size;
}

int HexBoard::getSize(){
  return size;
}

vector<Cell> HexBoard::getCells(){return Cells;}

void HexBoard::initialiseBoard(int size){
  for (int i = 0; i < size; ++i){
    for (int j = 0; j < size; ++j){
      vector<int> neighbors;
      if (i == 0){ //First row
        if (j == size - 1){ //Upper right corner
          neighbors = {i * size + j - 1, (i + 1) * size + j - 1, (i + 1) * size + j};
        }
        else if (j == 0){ //Upper left corner
          neighbors = {i * size + j + 1, (i + 1) * size + j};
        }
        else{ //Top row inner
          neighbors = {i * size + j - 1, (i + 1) * size + j - 1, (i + 1) * size + j, i * size + j + 1};
        }
      }
      else if (i == size - 1){ //Last Row
        if (j == 0){ //Lower left corner
          neighbors = {(i - 1) * size, (i - 1) * size + 1 , i * size + 1};
        }
        else if (j == size - 1){ //Bottom right corner
          neighbors = {i * size + j - 1, (i - 1) * size + j};
        }
        else {  //bottom row inner
          neighbors = {i * size + j - 1, (i - 1) * size + j, (i - 1) * size + j + 1, i * size + j + 1};
        }
      }
      else if(j == 0 && (i != 0 || i != size - 1)){  //Left column inner
        neighbors = {(i - 1) * size, (i - 1) * (size) + 1, i * size + 1, (i + 1) * size};
      }
      else if(j == size - 1 && (i != 0 || i != size - 1)){  //Right column inner
        neighbors = {(i - 1) * size + j, i * size + j - 1, (i + 1) * size + j - 1, (i + 1) * size + j};
      }
      else{ //Inner
        neighbors = {i * size + j - 1, (i - 1) * size + j, (i - 1) * size + j + 1, i * size + j + 1, (i + 1) * size + j, (i + 1) * size + j - 1};
      }
      Cells.at(i * size + j) = Cell(i, j, neighbors);
    }
  }
}

bool HexBoard::isLegal(int i, int j){
  if (i < 0 || i > size - 1 || j < 0 || j > size - 1 ){ //Check if coordinates are within the bounds
    //cout << "Move out of bounds" << endl;
    return false;
  }
  if (Cells.at(i * size + j).getPlayer() != 2){ //If the Cell is taken, attempt to make a move is illegal
    //cout << "Illegal Move" << endl;
    return false;
  }
  return true;
}

void HexBoard::playerMove(player p, int i, int j){
  if (isLegal(i , j)){ //Check legality of move
    Cells.at(i * size + j).setPlayer(p);
  }
  else{
    cout << "Illegal Move" << endl;
  }
}

void HexBoard::undoMove(int i, int j){
    Cells.at(i * size + j).setPlayer(EMPTY);
}

void HexBoard::simulatedMove(player p, int i, int j){
    Cells.at(i * size + j).setSimulated(p);
}

bool HexBoard::win(player p){
  bool endpoint = false;
  vector<bool> visited(size * size, false);
  stack<int> s;

  if (p == 0){
    for (int i = 0; i < size; ++i){
      if (Cells.at(i).getPlayer() == 0){ //Check if there are Cells filled on the north south ends
        for (int j = 0; j < size; ++j){
          if (Cells.at((size - 1) * size + j).getPlayer() == 0 || Cells.at((size - 1) * size + j).getSimulated() == 0){
            endpoint = true;
          }
        }
        break;
      }
    }

    // If no Cells at starting row/column or end row/column, no winner
    if (endpoint == false){
      return false;
    }

    //Perform DFS (From North to South)
    for (int i = 0; i < size; i++){
      if (Cells.at(i).getPlayer() == 0){
        s.push(i);
      }
    }

    while (!s.empty()){
      int c = s.top();
      if (Cells.at(c).I() == size - 1){ //Indicate that a path from North to south has been found. R has won the game
        return true;
      }
      s.pop();
      visited.at(c) = true;
      for (auto i : Cells.at(c).getNeighbors()){
        if (visited.at(i) == false && (Cells.at(i).getPlayer() == 0 || Cells.at(i).getSimulated() == 0)){
          s.push(i); //Push into stack if Cell is R, regardless of player move or simulated move
        }
      }
    }
  }
  else{
    for (int i = 0; i < size; ++i){
      if (Cells.at(i * size).getPlayer() == 1){ //Check if there are Cells filled on the East West ends
        for (int j = 0; j < size; ++j){
          if (Cells.at((j * size) + size - 1).getPlayer() == 1 || Cells.at((j * size) + size - 1).getSimulated() == 1){
            endpoint = true;
          }
        }
        break;
      }
    }

    // If no Cells at starting row/column or end row/column, no winner
    if (endpoint == false){
      return false;
    }

    //Push starting Cells at west side
    for (int i = 0; i < size; i++){
      if (Cells.at(i * size).getPlayer() == 1){
        s.push(i * size);
      }
    }

    //Perform DFS from east to west
    while (!s.empty()){
      int c = s.top();
      if (Cells.at(c).J() == size - 1){//Indicate that a path from West to East has been found. B has won the game
        return true;
      }
      s.pop();
      visited.at(c) = true;
      for (auto i : Cells.at(c).getNeighbors()){
        if (visited.at(i) == false && (Cells.at(i).getPlayer() == 1 || Cells.at(i).getSimulated() == 1)){
          s.push(i); //Push into stack if Cell is B, regardless of player move or simulated move
        }
      }
    }
  }
  //If no winner
  return false;
}

player HexBoard::simulate(HexBoard &h, player p, vector<tuple<int, int>> &Moves){
  //If human player is R then computer is B, vice versa
  player c;
  if (p == 0){
    c = B;
  }
  else{
    c = R;
  }

  //Copy moves
  auto legalMoves = Moves;
  random_shuffle(legalMoves.begin(), legalMoves.end());

  //While the board is not filled
  while (legalMoves.size() > 0){
    //Simulate human player making a move
    tuple<int, int> move = legalMoves.back();  //Get coordinates
    legalMoves.pop_back(); //Remove the move made from vector of legal moves
    h.simulatedMove(p, get<0>(move), get<1>(move));

    //If no remaining moves break
    if (legalMoves.size() == 0){
      break;
    }

    //Simulate computer making a move
    move = legalMoves.back();  //Get coordinates
    legalMoves.pop_back(); //Remove the move made from vector of legal moves
    h.simulatedMove(c, get<0>(move), get<1>(move));
  }


  if (h.win(p)){
    return p;
  }
  else if (h.win(c)){
    return c;
  }
  else{
    return EMPTY;
  }
}

void HexBoard::resetSimulation(){
  for (auto i = Cells.begin(); i != Cells.end(); i++){
    i->setSimulated(EMPTY);
  }
}

player HexBoard::computer(player c){
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  //If computer player is R then computer is B, vice versa
  player p;
  if (c == 0){
    p = B;
  }
  else{
    p = R;
  }

  //Get all the legal moves on the board
  vector<tuple<int, int>> legalMoves;
  for (int i = 0; i < getSize(); i++){
    for (int j = 0; j < getSize(); j++){
      if (isLegal(i, j)){
        legalMoves.push_back(tuple<int, int>{i, j});
        //cout << '(' << i << ", " << j << ")\t";
      }
    }
  }

  //Get copy of legal moves
  vector<tuple<int, int>> copyLegalMoves = legalMoves;
  auto iterator = copyLegalMoves.begin();


  //Iterate through the legal moves
  tuple<int, int> best_move;
  double best_probability = 0.0;
  HexBoard copyBoard = HexBoard(*this); //Copy the board
  int count = 0;
  for (auto i = legalMoves.begin(); i != legalMoves.end(); ++i){
    int wins = 0; //Keep track of the number of wins for each legal move
    copyBoard.playerMove(c, get<0>(*i), get<1>(*i)); //Make legal move
    iterator = copyLegalMoves.erase(iterator); //Erase the move from the copy

    //Simulate outcome
    for (int j = 0; j < 10000; ++j){	//Change the value to vary the number of simulations
      player winner = simulate(copyBoard, p, copyLegalMoves);
      if (winner == c){
        wins++;
      }
    }

    //Record current move if best move(defined by highest probability of winning) thus far
    double probability = static_cast<double>(wins) / 10000;
    if (probability > best_probability){
      best_move = *i;
      best_probability = probability;
    }

    //Undo the legal move
    copyBoard.undoMove(get<0>(*i), get<1>(*i));

    //Add back the legal move to copyLegalMoves
    copyLegalMoves.insert(iterator++, tuple<int, int>{get<0>(*i), get<1>(*i)});
  }

  //Make the best move
  cout << best_probability << endl;
  playerMove(c, get<0>(best_move), get<1>(best_move));
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Time difference = " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0) << "[s]" << std::endl;

  //Return winner if any
  if (win(c)){
    return c;
  }
  else{
    return EMPTY;
  }
}

void HexBoard::printHexBoard(){
  //First Line
  for (int i = 0; i < size; ++i){
    for (int j = 0; j < i * 2; j++){
      cout << ' ';
    }

    for (int j = 0; j < size; ++j){
      if (Cells.at(i * size + j).getPlayer() == 0){
        cout << "R ";
      }
      else if (Cells.at(i * size + j).getPlayer() == 1){
        cout << "B ";
      }
      else{
        cout << ". ";
      }
      if (j != size - 1){
        cout << "- ";
      }
    }
    cout << endl;
    if (i != size - 1){
      for (int j = 0; j < i * 2 + 1; ++j){
        cout << ' ';
      }
      for (int j = 0; j < size * 2 - 1; ++j){
        if (j % 2 == 0){
          cout << "\\ ";
        }
        else{
          cout << "/ ";
        }
      }
    }
    cout << endl;
  }
  cout << endl;
}
