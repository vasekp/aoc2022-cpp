#include <iostream>
#include <fstream>
#include <cmath>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  int time = 0;
  int var = 1;
  int total = 0;
  const auto commit = [&]() {
    int x = time % 40;
    if(x == 20) {
      total += var * time;
    }
    // The following lines = Part B.
    x = (time - 1) % 40;
    std::cout << (std::abs(var - x) <= 1 ? '#' : '.');
    if(x == 39)
      std::cout << '\n';
  };
  while(std::getline(file, line)) {
    if(line == "noop") {
      time += 1;
      commit();
    } else {
      int add;
      std::sscanf(line.c_str(), "addx %d", &add);
      time += 1;
      commit();
      time += 1;
      commit();
      var += add;
    }
  }
  std::cout << total << '\n';
}
