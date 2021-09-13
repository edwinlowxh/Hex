#include <vector>
using namespace std;

enum player{R, B, EMPTY};
class Cell{
public:
  Cell(int i = -1, int j = -1, vector<int> neighbors = {}, player p = EMPTY, player simulated = EMPTY);
  void setPlayer(player n);
  player getPlayer();
  void setSimulated(player n);
  player getSimulated();
  std::vector<int> getNeighbors();
  int J();
  int I();
private:
  vector<int> neighbors;
  int i;
  int j;
  player p;
  player simulated;
};
