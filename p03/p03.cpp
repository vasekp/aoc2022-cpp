#include <iostream>
#include <fstream>
#include <string_view>
#include <algorithm>

int prio(char c) {
  if(c >= 'a' && c <= 'z')
    return c - 'a' + 1;
  else
    return c - 'A' + 27;
}

char common(std::string_view a, std::string_view b) {
  return *std::find_if(a.begin(), a.end(), [b](char c) { return b.contains(c); });
}

char common(std::string_view a, std::string_view b, std::string_view c) {
  return *std::find_if(a.begin(), a.end(), [b, c](char x) { return b.contains(x) && c.contains(x); });
}

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
    while(std::getline(ifs, line)) {
      std::string_view a{line.begin(), line.begin() + line.size() / 2};
      std::string_view b{line.begin() + line.size() / 2, line.end()};
      total += prio(common(a, b));
    }
    std::cout << total << '\n';
  }

  {
    // Part B
    std::ifstream ifs{argv[1]};
    std::string line1, line2, line3;
    long total = 0;
    while(std::getline(ifs, line1)) {
      std::getline(ifs, line2);
      std::getline(ifs, line3);
      total += prio(common(line1, line2, line3));
    }
    std::cout << total << '\n';
  }
}
