#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <optional>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <numeric>

enum class Oper {
  plus, times
};

struct Monkey {
  std::vector<std::uint64_t> items;
  std::optional<std::uint64_t> op1;
  Oper oper;
  std::optional<std::uint64_t> op2;
  std::uint64_t div_test;
  std::size_t to_true;
  std::size_t to_false;
};

std::optional<std::uint64_t> old_or_num(const std::string& str) {
  return str == "old" ? std::optional<std::uint64_t>{} : stoi(str);
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string tmp;
  std::vector<Monkey> monkeys;
  std::uint64_t lcm = 1;
  while(file >> tmp) { // Monkey
    std::size_t monkey;
    std::uint64_t div_test;
    std::size_t to_true, to_false;
    std::vector<std::uint64_t> items{};
    assert(tmp == "Monkey");
    file >> monkey;
    assert(monkey == monkeys.size());
    file >> tmp >> tmp >> tmp; // : Starting items:
    assert(tmp == "items:");
    do {
      std::uint64_t item;
      file >> item;
      items.push_back(item);
      file >> tmp;
    } while(tmp == ",");
    assert(tmp == "Operation:");
    file >> tmp >> tmp; // new =
    std::string op1, oper, op2;
    file >> op1 >> oper >> op2;
    file >> tmp >> tmp >> tmp; // Test: divisible by
    assert(tmp == "by");
    file >> div_test;
    file >> tmp >> tmp; // If true:
    assert(tmp == "true:");
    file >> tmp >> tmp >> tmp >> to_true;
    file >> tmp >> tmp; // If false:
    assert(tmp == "false:");
    file >> tmp >> tmp >> tmp >> to_false;
    assert(to_true != monkey);
    assert(to_false != monkey);

    monkeys.push_back(Monkey{
        std::move(items),
        old_or_num(op1),
        oper == "+" ? Oper::plus : oper == "*" ? Oper::times : throw oper,
        old_or_num(op2),
        div_test,
        to_true,
        to_false
      });

    lcm = std::lcm(lcm, div_test);
  }

  std::vector<std::uint64_t> inspect;
  inspect.resize(monkeys.size());

  for(int round = 0; round < 10000; round++) {
    for(auto& monkey : monkeys) {
      inspect[std::distance(&monkeys[0], &monkey)] += monkey.items.size();
      for(auto item : monkey.items) {
        switch(monkey.oper) {
          case Oper::plus:
            item = monkey.op1.value_or(item) + monkey.op2.value_or(item);
            break;
          case Oper::times:
            item = monkey.op1.value_or(item) * monkey.op2.value_or(item);
            break;
        }
        item %= lcm;
        auto test = item % monkey.div_test == 0;
        monkeys[test ? monkey.to_true : monkey.to_false].items.push_back(item);
      }
      monkey.items.clear();
    }

    /*for(auto x : inspect)
      std::cout << x << '\n';
    std::cout << '\n';*/
  }

  std::sort(inspect.begin(), inspect.end(), std::greater<int>());
  std::cout << inspect[0] * inspect[1] << '\n';
}
