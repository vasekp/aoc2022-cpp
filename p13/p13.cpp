#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <variant>
#include <numeric>
#include <cassert>
#include <utility>
#include <algorithm>

struct Item;
using List = std::vector<Item>;

struct Item {
  std::variant<int, List> v;
};

std::ostream& operator<<(std::ostream& os, const List& list);

std::ostream& operator<<(std::ostream& os, const Item& item) {
  std::visit([&os](const auto& x) { os << x; }, item.v);
  return os;
}

std::ostream& operator<<(std::ostream& os, const List& list) {
  os << '[';
  for(const auto& item : list) {
    if(&item != &list[0])
      os << ',';
    os << item;
  }
  os << ']';
  return os;
}

std::istream& operator>>(std::istream& is, List& list);

std::istream& operator>>(std::istream& is, Item& item) {
  char c = is.get();
  //std::cout << "Item: " << c << '\n';
  is.unget();
  if(c == '[') {
    item.v = List{};
    is >> std::get<1>(item.v);
  } else {
    int num;
    is >> num;
    item.v = num;
  }
  return is;
}

std::istream& operator>>(std::istream& is, List& list) {
  list.clear();
  char c = is.get();
  //std::cout << "LB: " << c << '\n';
  assert(c == '[');
  c = is.get();
  //std::cout << "L1: " << c << '\n';
  if(c == ']')
    return is;
  else
    is.unget();
  do {
    Item i;
    is >> i;
    list.push_back(std::move(i));
    c = is.get();
    //std::cout << "LC: " << c << '\n';
  } while(c == ',');
  assert(c == ']');
  return is;
}

bool operator<(const Item& lhs, const Item& rhs);

bool operator<(const List& lhs, const List& rhs) {
  for(auto i = 0u; i < std::min(lhs.size(), rhs.size()); i++) {
    if(lhs[i] < rhs[i])
      return true;
    if(rhs[i] < lhs[i])
      return false;
  }
  return lhs.size() < rhs.size();
}

bool operator<(int lhs, const List& rhs) {
  return List{Item{lhs}} < rhs;
}

bool operator<(const List& lhs, int rhs) {
  return lhs < List{Item{rhs}};
}

bool operator<(const Item& lhs, const Item& rhs) {
  return std::visit([](const auto& l, const auto& r) -> bool { return l < r; }, lhs.v, rhs.v);
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  // Part A
  std::ifstream file{argv[1]};
  std::string line;
  int index = 1;
  int total = 0;
  std::vector<std::pair<Item, bool>> all;
  while(std::getline(file, line)) {
    Item lhs, rhs;
    std::istringstream{line} >> lhs;
    std::getline(file, line);
    std::istringstream{line} >> rhs;
    std::getline(file, line);
    assert(line.empty());
    if(lhs < rhs)
      total += index;
    // For Part B
    all.push_back({std::move(lhs), false});
    all.push_back({std::move(rhs), false});
    index++;
  }
  std::cout << total << '\n';

  // Part B
  all.push_back({Item{List{Item{List{Item{2}}}}}, true});
  all.push_back({Item{List{Item{List{Item{6}}}}}, true});
  std::sort(all.begin(), all.end());
  unsigned result = 1;
  for(auto i = 0u; i < all.size(); i++)
    if(all[i].second)
      result *= i + 1;
  std::cout << result << '\n';
}
