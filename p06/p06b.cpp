#include <iostream>
#include <tuple>
#include <cstring>

int main(int argc, char* argv[]) {
  if(argc != 1) {
    std::cerr << "Usage: " << argv[0] << " < input\n"; // Takes input from stdin
    return 1;
  }

  const unsigned LEN = 14; // Change to 4 to replicate Part A.
  char msg[LEN];
  unsigned reps = 0;
  for(auto i = 0u; i < LEN - 1; i++) {
    std::cin >> msg[i];
    for(auto j = 0u; j < i; j++)
      if(msg[i] == msg[j])
        reps++;
  }
  unsigned pos = LEN - 1;
  while(std::cin >> msg[LEN - 1]) {
    pos++;
    for(auto i = 0u; i < LEN - 1; i++) {
      if(msg[LEN - 1] == msg[i])
        reps++;
    }
    if(reps == 0) {
      std::cout << pos << '\n';
      return 0;
    }
    for(auto i = 1u; i < LEN; i++) {
      if(msg[i] == msg[0])
        reps--;
    }
    std::memmove(&msg[0], &msg[1], LEN - 1);
  }
}
