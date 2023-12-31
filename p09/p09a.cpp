#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <string>
#include <cstdio>

void move(std::pair<int, int>& tail, const std::pair<int, int>& head) {
  if(head.first - tail.first == 2) {
    tail.first += 1;
    tail.second = head.second;
  } else if(head.first - tail.first == -2) {
    tail.first -= 1;
    tail.second = head.second;
  } else if(head.second - tail.second == -2) {
    tail.second -= 1;
    tail.first = head.first;
  } else if(head.second - tail.second == 2) {
    tail.second += 1;
    tail.first = head.first;
  }
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  std::pair<int, int> head = {0, 0}, tail = {0, 0};
  std::set<std::pair<int, int>> visited{};
  while(std::getline(file, line)) {
    char dir;
    int dist;
    std::sscanf(line.c_str(), "%c %d", &dir, &dist);
    for(int i = 0; i < dist; i++) {
      switch(dir) {
        case 'R':
          head.first += 1;
          break;
        case 'L':
          head.first -= 1;
          break;
        case 'U':
          head.second -= 1;
          break;
        case 'D':
          head.second += 1;
          break;
      }
      move(tail, head);
      //std::cout << head.first << ' ' << head.second << ' ' << tail.first << ' ' << tail.second << '\n';
      visited.insert(tail);
    }
  }
  std::cout << visited.size() << '\n';
}
