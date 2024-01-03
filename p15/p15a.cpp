#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <limits>

int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " input y-coord\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  int yt = std::atoi(argv[2]);
  std::string line;
  std::set<int> beacons_in_target{};
  std::vector<std::pair<int, int>> covered{};
  while(std::getline(file, line)) {
    int xs, ys, xb, yb;
    std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &xs, &ys, &xb, &yb);
    if(yb == yt)
      beacons_in_target.insert(xb);
    int dist = std::abs(xb - xs) + std::abs(yb - ys);
    if(int yd = std::abs(ys - yt); dist >= yd) {
      int xm = xs - (dist - yd);
      int xp = xs + (dist - yd);
      covered.push_back({xm, xp});
    }
  }
  std::sort(covered.begin(), covered.end());
  int total = 0;
  int last_end = std::numeric_limits<int>::min();
  for(const auto& pair : covered) {
    if(pair.first > last_end)
      total += pair.second - pair.first + 1;
    else if(pair.second > last_end)
      total += pair.second - last_end;
    else
      continue;
    last_end = pair.second;
  }
  std::cout << total - beacons_in_target.size() << '\n';
}
