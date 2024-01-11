#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <array>
#include <algorithm>
#include <numeric>
#include <deque>

using Triple = std::array<int, 3>;

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  std::string line;
  std::set<Triple> blocks{};
  while(std::getline(file, line)) {
    std::istringstream iss{line};
    int x, y, z;
    char c;
    iss >> x >> c >> y >> c >> z;
    blocks.insert({x, y, z});
  }

  auto free = [&blocks](int x, int y, int z) {
    return !blocks.contains({x, y, z});
  };

  // Part A
  unsigned faces = 0;
  std::for_each(blocks.begin(), blocks.end(), [&](const Triple& pos) {
      faces += free(pos[0] - 1, pos[1], pos[2])
        + free(pos[0] + 1, pos[1], pos[2])
        + free(pos[0], pos[1] - 1, pos[2])
        + free(pos[0], pos[1] + 1, pos[2])
        + free(pos[0], pos[1], pos[2] - 1)
        + free(pos[0], pos[1], pos[2] + 1);
    });
  std::cout << faces << '\n';

  // Part B
  auto min = std::reduce(blocks.begin(), blocks.end(), *blocks.begin(),
      [](const Triple& a, const Triple& b) -> Triple {
    return {std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2])};
  });
  auto max = std::reduce(blocks.begin(), blocks.end(), *blocks.begin(),
      [](const Triple& a, const Triple& b) -> Triple {
    return {std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])};
  });

  for(auto& x : min) x -= 1;
  for(auto& x : max) x += 1;
  std::set<Triple> surround{};
  std::deque queue{min};
  while(!queue.empty()) {
    auto tri = queue.front();
    queue.pop_front();
    if(blocks.contains(tri) || surround.contains(tri))
      continue;
    surround.insert(tri);
    if(tri[0] > min[0])
      queue.push_back({tri[0] - 1, tri[1], tri[2]});
    if(tri[0] < max[0])
      queue.push_back({tri[0] + 1, tri[1], tri[2]});
    if(tri[1] > min[1])
      queue.push_back({tri[0], tri[1] - 1, tri[2]});
    if(tri[1] < max[1])
      queue.push_back({tri[0], tri[1] + 1, tri[2]});
    if(tri[2] > min[2])
      queue.push_back({tri[0], tri[1], tri[2] - 1});
    if(tri[2] < max[2])
      queue.push_back({tri[0], tri[1], tri[2] + 1});
  }

  unsigned out_faces = 0;
  std::for_each(surround.begin(), surround.end(), [&](const Triple& pos) {
      out_faces += !free(pos[0] - 1, pos[1], pos[2])
        + !free(pos[0] + 1, pos[1], pos[2])
        + !free(pos[0], pos[1] - 1, pos[2])
        + !free(pos[0], pos[1] + 1, pos[2])
        + !free(pos[0], pos[1], pos[2] - 1)
        + !free(pos[0], pos[1], pos[2] + 1);
    });
  std::cout << out_faces << '\n';
}
