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

struct Node {
  int rate;
  std::vector<std::string> tunnels;
};

constexpr unsigned limit = 32;
constexpr int time_alloc = 30;

std::pair<std::string, int> find_best(std::bitset<limit> left, std::string cur, int time,
    const std::map<std::string, Node>& nodes,
    const std::vector<std::string>& valve_nodes, int valve_rem,
    const std::map<std::pair<std::string, std::string>, int>& distances,
    std::map<std::pair<unsigned, std::string>, std::pair<std::string, int>>& known);

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
    //std::cout << node << ' ' << rate << '\n';
    std::vector<std::string> tunnels;
    while(iss >> tmp) {
      auto to = (tmp[tmp.size() - 1] == ',')
        ? std::move(tmp).substr(0, tmp.size() - 1) : std::move(tmp);
      //std::cout << node << " -> " << to << '\n';
      tunnels.push_back(std::move(to));
    }
    nodes[node] = Node{rate, std::move(tunnels)};
    if(rate) {
      valve_nodes.push_back(node);
      valve_sum += rate;
    }
  }

  std::map<std::pair<std::string, std::string>, int> distances{};
  for(const auto& [n1, node1] : nodes) {
    //for(const auto& n2 : valve_nodes) {
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

  assert(valve_nodes.size() + 1 < limit);
  std::map<std::pair<unsigned, std::string>, std::pair<std::string, int>> best{};
  auto [next, cost] = find_best({}, "AA", 0, nodes, valve_nodes, valve_sum, distances, best);
  //std::cout << n << ' ' << c << '\n';
  std::cout << time_alloc * valve_sum - cost << '\n';
}

// A solution based on the premise that all nodes are reached on the optimal path, i.e., within the time allocation.
// This is true for 'test', for which this algorithm performs much faster (2^N as opposed to N! complexity),
// but it fails for 'input'. We need to resort to brute force :-(
std::pair<std::string, int> find_best(std::bitset<limit> seen, std::string cur, int time,
    const std::map<std::string, Node>& nodes,
    const std::vector<std::string>& valve_nodes, int valve_rem,
    const std::map<std::pair<std::string, std::string>, int>& distances,
    std::map<std::pair<unsigned, std::string>, std::pair<std::string, int>>& known) {
  assert(time <= time_alloc); // Unfortunately this assertion is wrong.
  if(known.contains({seen.to_ulong(), cur}))
    return known.at({seen.to_ulong(), cur});
  int best_cost = 0;
  std::string best_next{};
  auto ix = std::distance(valve_nodes.begin(), std::find(valve_nodes.begin(), valve_nodes.end(), cur));
  // It's fine if cur is not a valve_node; then the above just gives .size().
  seen.set(ix);
  for(auto i = 0u; i < valve_nodes.size(); i++) {
    if(seen.test(i))
      continue;
    auto next = valve_nodes[i];
    auto d_plus_one = distances.at({cur, next}) + 1;
    auto base_cost = d_plus_one * valve_rem;
    auto [next2, cost2] = find_best(seen, next, time + d_plus_one, nodes,
        valve_nodes, valve_rem - nodes.at(next).rate, distances, known);
    auto cost = base_cost + cost2;
    if(best_cost == 0 || cost < best_cost) {
      best_cost = cost;
      best_next = next;
    }
  }
  seen.reset(ix);
  //std::cout << seen.to_string() << ", " << cur << ": " << best_next << ", " << best_cost << '\n';
  known[{seen.to_ulong(), cur}] = {best_next, best_cost};
  return {best_next, best_cost};
}
