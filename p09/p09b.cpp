#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <string>
#include <cstdio>
#include <array>
#include <algorithm>

void move(std::pair<int, int>& tail, const std::pair<int, int>& head) {
  constexpr auto sgn = [](int x) { return x == 0 ? 0 : x < 0 ? -1 : 1; };
  if(std::max(std::abs(head.first - tail.first), std::abs(head.second - tail.second)) == 2) {
    tail.first += sgn(head.first - tail.first);
    tail.second += sgn(head.second - tail.second);
  }
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  constexpr unsigned LEN = 10; // Change to 2 to replicate Part A.
  std::array<std::pair<int, int>, LEN> rope{};
  std::set<std::pair<int, int>> visited{};
  while(std::getline(file, line)) {
    char dir;
    int dist;
    std::sscanf(line.c_str(), "%c %d", &dir, &dist);
    for(int i = 0; i < dist; i++) {
      switch(dir) {
        case 'R':
          rope[0].first += 1;
          break;
        case 'L':
          rope[0].first -= 1;
          break;
        case 'U':
          rope[0].second -= 1;
          break;
        case 'D':
          rope[0].second += 1;
          break;
      }
      for(auto j = 1u; j < LEN; j++)
        move(rope[j], rope[j - 1]);
      visited.insert(rope[LEN - 1]);
    }
  }
  std::cout << visited.size() << '\n';
}
