#include <iostream>
#include <fstream>
#include <cstdio>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream ifs{argv[1]};
  std::string line;
  long count1 = 0, count2 = 0;
  while(std::getline(ifs, line)) {
    int f1, t1, f2, t2;
    std::sscanf(line.c_str(), "%d-%d,%d-%d", &f1, &t1, &f2, &t2);
    if((f2 >= f1 && t2 <= t1) || (f1 >= f2 && t1 <= t2))
      count1++;
    if((f2 >= f1 && f2 <= t1) || (t2 >= f1 && t2 <= t1) || (f1 >= f2 && f1 <= t2) || (t1 >= f2 && t1 <= t2))
      count2++;
  }
  // Part A
  std::cout << count1 << '\n';
  // Part B
  std::cout << count2 << '\n';
}
