#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <set>
#include <utility>
#include <cmath>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  // Observation: no straight line longer than 20 px ⇒ safe to expand to individual points
  std::set<std::pair<int, int>> set{};
  int ymax = 0;
  while(std::getline(file, line)) {
    std::istringstream iss{line};
    std::pair<int, int> last;
    char c;
    std::string arrw;
    iss >> last.first >> c >> last.second;
    assert(c == ',');
    while(iss >> arrw) {
      assert(arrw == "->");
      std::pair<int, int> cur;
      iss >> cur.first >> c >> cur.second;
      assert(c == ',');
      auto xd = std::abs(last.first - cur.first),
           yd = std::abs(last.second - cur.second);
      assert(xd == 0 || yd == 0);
      auto xm = std::min(last.first, cur.first),
           ym = std::min(last.second, cur.second);
      if(xd == 0)
        for(auto y = ym; y <= ym + yd; y++)
          set.insert({xm, y});
      else
        for(auto x = xm; x <= xm + xd; x++)
          set.insert({x, ym});
      ymax = std::max(ymax, ym + yd);
      last = cur;
    }
  }
  //std::cout << set.size() << ' ' << ymax << '\n';

  int part_a = 0, part_b = 0;
  for(auto time = 0; ; time++) {
    int x = 500;
    int y = 0;
    for(;;) {
      if(y > ymax) {
        // Part A would stop here
        if(!part_a)
          part_a = time;
        set.insert({x, y});
        break;
      } else if(!set.contains({x, y + 1})) {
        y += 1;
      } else if(!set.contains({x - 1, y + 1})) {
        y += 1;
        x -= 1;
      } else if(!set.contains({x + 1, y + 1})) {
        y += 1;
        x += 1;
      } else {
        //std::cout << x << ',' << y << '\n';
        set.insert({x, y});
        // Part B stops here
        if(x == 500 && y == 0)
          part_b = time + 1;
        break;
      }
    }
    if(part_b)
      break;
  }
  std::cout << part_a << '\n' << part_b << '\n';
}
