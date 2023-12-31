#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream ifs{argv[1]};
  std::string line;
  std::vector<long> vec{};
  long total = 0;
  auto proc = [&vec](auto total) {
    vec.push_back(total);
    std::sort(vec.begin(), vec.end(), std::greater<long>());
    if(vec.size() > 3)
      vec.resize(3);
  };
  while(std::getline(ifs, line)) {
    if(line.empty()) {
      proc(total);
      total = 0;
      continue;
    }
    total += std::stol(line);
    //std::cout << line << ' ' << v << '\n';
  }
  proc(total);
  // Part A
  std::cout << vec.front() << '\n';
  // Part B
  std::cout << std::accumulate(vec.begin(), vec.end(), 0l) << '\n';
}
