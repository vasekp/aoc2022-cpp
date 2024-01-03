#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>

using Pair = std::pair<int, int>;

int dist(const Pair& a, const Pair& b) {
  return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " input y-coord (ignored)\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  //int yt = std::atoi(argv[2]);
  std::string line;
  std::vector<std::pair<std::pair<int, int>, int>> sensors{};
  while(std::getline(file, line)) {
    int xs, ys, xb, yb;
    std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &xs, &ys, &xb, &yb);
    sensors.push_back({{xs, ys}, dist({xb, yb}, {xs, ys})});
  }

  // Infinitely long lines along diagonals delimiting sensors' areas
  std::set<int> sums{}, diffs{};
  for(const auto& [center, dist] : sensors) {
    auto sum = center.first + center.second;
    auto diff = center.first - center.second;
    sums.insert(sum + dist);
    sums.insert(sum - dist);
    diffs.insert(diff + dist);
    diffs.insert(diff - dist);
  }

  // Promised there is a single uncovered field, there has to be a pair of lines at distance 2 in each direction
  std::erase_if(sums, [&](int x) { return !sums.contains(x + 2); });
  std::erase_if(diffs, [&](int x) { return !diffs.contains(x + 2); });

  auto check = [&](const Pair& p) {
    return std::all_of(sensors.begin(), sensors.end(), [&p](const auto& beacon) { return dist(p, beacon.first) > beacon.second; });
  };

  // All points at such diagonal-antidiagonal crossings that are not covered
  std::vector<Pair> candidates{};
  for(auto s : sums)
    for(auto d : diffs) {
      if((s + d) & 1)
        continue;
      Pair p{(s + d + 2) / 2, (s - d) / 2};
      if(check(p))
        candidates.push_back(p);
    }

  // The above may contain a stray point outside of the full region, but that will pass check alongside some of its neighbours.
  // By filtering on this we single out inside solutions.
  std::erase_if(candidates, [&](const Pair& p) {
      auto [x, y] = p;
      return check({x - 1, y}) || check({x + 1, y}) || check({x, y - 1}) || check({x, y + 1});
    });
  assert(candidates.size() == 1);

  auto p = candidates.front();
  std::cout << (p.first * 4000000L + p.second) << '\n';
}
