#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstdint>
#include <algorithm>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  std::vector<std::string> cd{};
  std::map<decltype(cd), std::uint64_t> map{};
  while(std::getline(file, line)) {
    if(line[0] == '$') {
      if(!line.compare(0, 5, "$ cd ")) {
        auto dir = line.substr(5);
        if(dir == "/")
          cd.clear();
        else if(dir == "..")
          cd.pop_back();
        else
          cd.push_back(dir);
      } else if(!line.compare(0, 5, "$ ls ")) {
        map[cd] = 0;
      }
    } else if(!line.compare(0, 4, "dir "))
      continue;
    else {
      std::uint64_t s;
      std::sscanf(line.c_str(), "%lu", &s);
      map[cd] += s;
    }
  }
  std::map<decltype(cd), std::uint64_t> map2{};
  for(auto [dir, s] : map) {
    auto dir2 = dir;
    for(;;) {
      map2[dir2] += s;
      if(dir2.empty()) break;
      dir2.pop_back();
    }
  }

  std::uint64_t total = 0;
  const std::uint64_t THRES = 100000;
  for(const auto& [dir, s] : map2) {
    //std::cout << (dir.empty() ? "/" : dir.back()) << ' ' << s << '\n';
    if(s <= THRES)
      total += s;
  }
  std::cout << total << '\n';

  const std::uint64_t TOTAL = 70000000, NEEDED = 30000000;
  auto all = map2[{}];
  auto need_free = NEEDED - (TOTAL - all);
  //std::cout << need_free << '\n';
  std::uint64_t min_found = TOTAL;
  for(const auto& [dir, s] : map2) {
    if(s < need_free)
      continue;
    if(s < min_found)
      min_found = s;
  }
  std::cout << min_found << '\n';
}
