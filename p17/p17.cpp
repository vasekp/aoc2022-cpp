#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <utility>
#include <optional>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input\n";
    return 1;
  }

  std::string dirs{};
  {
    std::ifstream file{argv[1]};
    file >> dirs;
  }

  std::array<std::vector<std::uint8_t>, 5> tiles = {
    std::vector<std::uint8_t>{{0b1111}},
    std::vector<std::uint8_t>{{0b010, 0b111, 0b010}},
    std::vector<std::uint8_t>{{0b111, 0b001, 0b001}},
    std::vector<std::uint8_t>{{0b1, 0b1, 0b1, 0b1}},
    std::vector<std::uint8_t>{{0b11, 0b11}}
  };
  std::array<int, 5> widths = {4, 3, 3, 1, 2};

  std::vector<std::uint8_t> field{};
  auto tile_index = 0u;
  auto dir_index = 0u;
  std::size_t tiles_down = 0;
  // For Part B
  std::map<std::pair<unsigned, unsigned>, std::vector<std::pair<std::size_t, std::size_t>>> loop_starts{};
  std::optional<std::size_t> period_tiles{};
  std::optional<std::size_t> period_height{};
  for(;;) {
    // Insert new tile
    const auto& tile = tiles[tile_index];
    auto tile_width = widths[tile_index];
    auto tile_height = tile.size();
    auto tile_y = field.size() + 3;
    auto tile_x = 2;
    auto rem_width = 7 - tile_width;
    if(++tile_index >= tiles.size())
      tile_index = 0;

    auto can_fit = [&](unsigned x0, unsigned y0) {
      for(auto y = 0u; y < tile.size(); y++) {
        if(y + y0 >= field.size())
          return true;
        if((tile[y] << (rem_width - x0)) & field[y + y0])
          return false;
      }
      return true;
    };

    for(;;) {
      switch(dirs[dir_index]) {
        case '<':
          if(tile_x > 0 && can_fit(tile_x - 1, tile_y))
            tile_x -= 1;
          break;
        case '>':
          if(tile_x < rem_width && can_fit(tile_x + 1, tile_y))
            tile_x += 1;
          break;
      }
      if(++dir_index >= dirs.size())
        dir_index = 0;
      if(tile_y > 0 && can_fit(tile_x, tile_y - 1)) {
        tile_y -= 1;
      } else {
        field.resize(std::max(field.size(), tile_y + tile_height));
        for(auto y = 0u; y < tile.size(); y++)
          field[tile_y + y] |= tile[y] << (rem_width - tile_x);
        tiles_down += 1;
        break;
      }
    }

    // Part A
    if(tiles_down == 2022)
      std::cout << field.size() << '\n';

    // Part B
    if(period_tiles && tiles_down > 2022) {
      constexpr std::size_t target = 1000000000000;
      if((target - tiles_down) % period_tiles.value() == 0) {
        auto periods = (target - tiles_down) / period_tiles.value();
        auto target_height = field.size() + periods * period_height.value();
        std::cout << target_height << '\n';
        return 0;
      }
    } else {
      if(loop_starts.contains({dir_index, tile_index})) {
        auto& vec = loop_starts.at({dir_index, tile_index});
        vec.push_back({tiles_down, field.size()});
        if(auto size = vec.size(); size > 2) {
          const auto& u = vec[size - 1];
          const auto& v = vec[size - 2];
          const auto& w = vec[size - 3];
          if(u.first - v.first == v.first - w.first && u.second - v.second == v.second - w.second) {
            period_tiles = u.first - v.first;
            period_height = u.second - v.second;
          }
        }
      } else {
        loop_starts.insert({{dir_index, tile_index}, {std::pair<std::size_t, std::size_t>{tiles_down, field.size()}}});
      }
    }
  }
}
