#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <bitset>
#include <cassert>
#include <algorithm>
#include <limits>
#include <functional>

struct Node {
  int rate;
  std::vector<std::string> tunnels;
};

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  std::map<std::string, Node> nodes{};
  std::vector<std::string> valve_nodes{};
  int valve_sum = 0;
  while(std::getline(file, line)) {
    std::string node;
    std::istringstream iss{line};
    std::string tmp;
    iss >> tmp; // Valve
    iss >> node;
    iss >> tmp >> tmp >> tmp; // has flow rate=...
    int rate = std::atoi(tmp.c_str() + 5);
    iss >> tmp >> tmp >> tmp >> tmp; // tunnels lead to valves
    std::vector<std::string> tunnels;
    while(iss >> tmp) {
      auto to = (tmp[tmp.size() - 1] == ',')
        ? std::move(tmp).substr(0, tmp.size() - 1) : std::move(tmp);
      tunnels.push_back(std::move(to));
    }
    nodes[node] = Node{rate, std::move(tunnels)};
    if(rate) {
      valve_nodes.push_back(node);
      valve_sum += rate;
    }
  }

  constexpr auto limit = std::numeric_limits<unsigned>::digits;
  assert(valve_nodes.size() + 1 < limit);

  std::map<std::pair<std::string, std::string>, int> distances{};
  for(const auto& [n1, node1] : nodes) {
    std::deque<std::pair<std::string, int>> queue{{n1, 0}};
    while(!queue.empty()) {
      auto [n2, d] = queue.front();
      queue.pop_front();
      if(distances.contains({n1, n2}))
        continue;
      distances[{n1, n2}] = d;
      const auto& node = nodes.at(n2);
      for(const auto& to : node.tunnels)
        queue.push_back({to, d + 1});
    }
  }

  /*for(const auto& n1 : valve_nodes)
    for(const auto& n2 : valve_nodes)
      if(n2 != n1)
        std::cout << n1 << "->" << n2 << ": " << distances.at({n1, n2}) << '\n';
  for(const auto& n2 : valve_nodes)
    std::cout << "AA" << "->" << n2 << ": " << distances.at({"AA", n2}) << '\n';*/

  // Part A
  std::function<int(std::bitset<limit>, std::string, int)> find_best =
      [&](std::bitset<limit> seen, std::string cur, int time_left) {
    auto ix = std::distance(valve_nodes.begin(), std::find(valve_nodes.begin(), valve_nodes.end(), cur));
    // It's fine if cur is not a valve_node; then the above just gives .size().
    seen.set(ix);
    int best = 0;
    for(auto i = 0u; i < valve_nodes.size(); i++) {
      if(seen.test(i))
        continue;
      auto next = valve_nodes[i];
      auto d_plus_one = distances.at({cur, next}) + 1;
      if(d_plus_one <= time_left) {
        auto value = (time_left - d_plus_one) * nodes.at(next).rate
          + find_best(seen, next, time_left - d_plus_one);
        if(value > best)
          best = value;
      }
    }
    //seen.reset(ix); // Needless except for debugging
    return best;
  };
  std::cout << find_best({}, "AA", 30) << '\n';

  // Part B – brute force, runs for a couple minutes.
  std::function<int(std::bitset<limit>, std::string, int, std::string, int)> find_best_two =
      [&](std::bitset<limit> seen, std::string cur1, int time1, std::string cur2, int time2) {
    auto ix1 = std::distance(valve_nodes.begin(), std::find(valve_nodes.begin(), valve_nodes.end(), cur1));
    auto ix2 = std::distance(valve_nodes.begin(), std::find(valve_nodes.begin(), valve_nodes.end(), cur2));
    int best = 0;
    seen.set(ix1);
    seen.set(ix2);
    // Option 1: both make a step
    for(auto i = 0u; i < valve_nodes.size(); i++) {
      if(seen.test(i))
        continue;
      for(auto j = 0u; j < valve_nodes.size(); j++) {
        if(i == j || seen.test(j))
          continue;
        auto next1 = valve_nodes[i];
        auto d1 = distances.at({cur1, next1}) + 1;
        auto next2 = valve_nodes[j];
        auto d2 = distances.at({cur2, next2}) + 1;
        if(d1 <= time1 && d2 <= time2) {
          auto value = (time1 - d1) * nodes.at(next1).rate
            + (time2 - d2) * nodes.at(next2).rate
            + find_best_two(seen, next1, time1 - d1, next2, time2 - d2);
          if(value > best)
            best = value;
        }
      }
    }
    // Option 2: one of them does all the remaining steps
    // We don't return to the other agent from this point, because if there was a path in which
    // it took another step, that would also be covered by option 1.
    for(auto i = 0u; i < valve_nodes.size(); i++) {
      if(seen.test(i))
        continue;
      auto next = valve_nodes[i];
      auto d_plus_one = distances.at({cur1, next}) + 1;
      if(d_plus_one <= time1) {
        auto value = (time1 - d_plus_one) * nodes.at(next).rate
          + find_best(seen, next, time1 - d_plus_one);
        if(value > best)
          best = value;
      }
      d_plus_one = distances.at({cur2, next}) + 1;
      if(d_plus_one <= time2) {
        auto value = (time2 - d_plus_one) * nodes.at(next).rate
          + find_best(seen, next, time2 - d_plus_one);
        if(value > best)
          best = value;
      }
    }
    //seen.reset(ix1);
    //seen.reset(ix2);
    return best;
  };
  std::cout << find_best_two({}, "AA", 26, "AA", 26) << '\n';
}
