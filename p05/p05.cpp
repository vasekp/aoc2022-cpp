#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <cstdio>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream ifs{argv[1]};
  std::string line;
  std::vector<std::vector<char>> crates{};
  while(std::getline(ifs, line)) {
    if(crates.empty())
      crates.resize((line.size() + 1) / 4);
    if(std::isdigit(line[1]))
      break;
    for(auto i = 1u, j = 0u; i < line.size(); i += 4, j++)
      if(line[i] != ' ')
        crates[j].insert(crates[j].begin(), line[i]);
  }

  std::getline(ifs, line);
  assert(line.empty());

  while(std::getline(ifs, line)) {
    unsigned count, from, to;
    std::sscanf(line.c_str(), "move %u from %u to %u", &count, &from, &to);
    from -= 1;
    to -= 1;
    // commented for part B :-) Uncomment for Part A.
    //std::reverse(crates[from].end() - count, crates[from].end());
    crates[to].insert(crates[to].end(), crates[from].end() - count, crates[from].end());
    crates[from].erase(crates[from].end() - count, crates[from].end());
  }

  for(auto& v : crates)
    std::cout << v.back();
  std::cout << '\n';
}
