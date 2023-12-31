#include <iostream>
#include <tuple>

int main(int argc, char* argv[]) {
  if(argc != 1) {
    std::cerr << "Usage: " << argv[0] << " < input\n"; // Takes input from stdin
    return 1;
  }

  char a, b, c, d;
  unsigned pos;
  std::cin >> a >> b >> c;
  pos = 3;
  while(std::cin >> d) {
    pos++;
    if(d != a && d != b && d != c && c != b && c != a && b != a) {
      std::cout << pos << '\n';
      return 0;
    }
    std::tie(a, b, c) = std::tie(b, c, d);
  }
}
