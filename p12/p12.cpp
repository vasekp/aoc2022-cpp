#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <tuple>
#include <map>
#include <numeric>
#include <functional>

using Pair = std::array<std::size_t, 2>;

std::size_t dfs(const std::vector<std::string>& field, Pair start,
    const std::function<bool(char, char)>& condition,
    const std::function<bool(Pair)>& test) {
  auto height = field.size();
  auto width = field[0].size();
  std::map<Pair, std::size_t> map{};
  std::deque<std::tuple<Pair, std::size_t, char>> queue{{start, 0, field[start[1]][start[0]]}};
  while(!queue.empty()) {
    auto [pos, len, last] = queue.front();
    queue.pop_front();
    auto [x, y] = pos;
    auto cur = field[y][x];
    if(!condition(last, cur) || map.contains(pos))
      continue;
    if(test(pos))
      return len;
    map[pos] = len;
    if(x > 0)
      queue.push_back({{x - 1, y}, len + 1, cur});
    if(x + 1 < width)
      queue.push_back({{x + 1, y}, len + 1, cur});
    if(y > 0)
      queue.push_back({{x, y - 1}, len + 1, cur});
    if(y + 1 < height)
      queue.push_back({{x, y + 1}, len + 1, cur});
  }
  throw "not found!";
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  std::vector<std::string> field{};
  Pair start, end;
  std::size_t y = 0;
  while(std::getline(file, line)) {
    if(auto x = line.find('S'); x != std::string::npos) {
      line[x] = 'a';
      start = {x, y};
    }
    if(auto x = line.find('E'); x != std::string::npos) {
      line[x] = 'z';
      end = {x, y};
    }
    field.push_back(std::move(line));
    y++;
  }

  // Part A
  std::cout << dfs(field, start,
      [](char a, char b) { return b <= a + 1; },
      [&](Pair p) { return p == end; }) << '\n';

  // Part B
  std::cout << dfs(field, end,
      [](char a, char b) { return b >= a - 1; },
      [&](Pair p) { return field[p[1]][p[0]] == 'a'; }) << '\n';
}
