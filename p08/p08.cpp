#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::vector<std::vector<std::pair<int, bool>>> field{};
  std::string line;
  while(std::getline(file, line)) {
    std::vector<std::pair<int, bool>> row{};
    for(char c : line)
      row.push_back({c - '0', false});
    field.push_back(std::move(row));
  }
  auto rows = field.size();
  auto cols = field[0].size();

  // Part A
  for(auto y = 0u; y < rows; y++) {
    int h = -1;
    for(auto x = 0u; x < cols; x++) {
      auto& pair = field[y][x];
      if(pair.first > h) {
        pair.second = true;
        h = pair.first;
      }
    }
  }
  for(auto y = 0u; y < rows; y++) {
    int h = -1;
    for(auto x = 0u; x < cols; x++) {
      auto& pair = field[y][cols - 1 - x];
      if(pair.first > h) {
        pair.second = true;
        h = pair.first;
      }
    }
  }
  for(auto x = 0u; x < cols; x++) {
    int h = -1;
    for(auto y = 0u; y < rows; y++) {
      auto& pair = field[y][x];
      if(pair.first > h) {
        pair.second = true;
        h = pair.first;
      }
    }
  }
  for(auto x = 0u; x < cols; x++) {
    int h = -1;
    for(auto y = 0u; y < rows; y++) {
      auto& pair = field[rows - 1 - y][x];
      if(pair.first > h) {
        pair.second = true;
        h = pair.first;
      }
    }
  }
  auto count = 0u;
  for(const auto& row : field)
    for(const auto& pair : row)
      if(pair.second)
        count++;
  std::cout << count << '\n';

  // Part B
  unsigned max = 0;
  for(auto y = 0u; y < rows; y++) {
    for(auto x = 0u; x < cols; x++) {
      auto h = field[y][x].first;
      unsigned d1 = 0, d2 = 0, d3 = 0, d4 = 0;
      for(auto x2 = x + 1; x2 < cols; x2++) {
        d1++;
        if(field[y][x2].first >= h)
          break;
      }
      for(int x2 = x - 1; x2 >= 0; x2--) {
        d2++;
        if(field[y][x2].first >= h)
          break;
      }
      for(auto y2 = y + 1; y2 < rows; y2++) {
        d3++;
        if(field[y2][x].first >= h)
          break;
      }
      for(int y2 = y - 1; y2 >= 0; y2--) {
        d4++;
        if(field[y2][x].first >= h)
          break;
      }
      //std::cout << x << ' ' << y << ' ' << h << ' ' << d1 << ' ' << d2 << ' ' << d3 << ' ' << d4 << '\n';
      auto score = d1 * d2 * d3 * d4;
      if(score > max)
        max = score;
    }
  }
  std::cout << max << '\n';
}
