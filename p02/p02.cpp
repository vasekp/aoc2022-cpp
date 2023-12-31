#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  {
    // Part A
    std::ifstream ifs{argv[1]};
    std::string line;
    long total = 0;
    auto score = [](char a, char b) {
      int x = a - 'A', y = b - 'X';
      // y == x-1 => lose => 0
      // y == x   => draw => 1
      // y == x+1 => win  => 2
      int res = (4 + y - x) % 3;
      return res * 3 + y + 1;
    };
    while(std::getline(ifs, line))
      total += score(line[0], line[2]);
    std::cout << total << '\n';
  }

  {
    // Part B
    std::ifstream ifs{argv[1]};
    std::string line;
    long total = 0;
    auto score = [](char a, char b) {
      int x = a - 'A', y = b - 'X';
      // y == 0 => lose => z = x-1
      // y == 1 => draw => z = x
      // y == 2 => win  => z = x+1
      int ret = (2 + x + y) % 3;
      return y * 3 + ret + 1;
    };
    while(std::getline(ifs, line))
      total += score(line[0], line[2]);
    std::cout << total << '\n';
  }
}
