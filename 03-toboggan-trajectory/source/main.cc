#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using TreeMask = std::vector<std::string>;

TreeMask ParseTreeMask(char const *path) {
  TreeMask mask;
  auto stream = std::ifstream(path);
  std::string line;
  while (std::getline(stream, line))
    mask.push_back(line);
  return mask;
}

size_t CountTreesOnSlope(TreeMask const &mask, size_t dx, size_t dy) {
  auto height = mask.size();
  auto width = mask.at(0).size();

  size_t trees_encountered = 0;
  for (size_t x = dx, y = dy; y < height; x += dx, y += dy)
    trees_encountered += (mask.at(y).at(x % width) == '#');
  return trees_encountered;
}

int main(int argc, char const **argv) {
  assert(argc == 2);
  auto mask = ParseTreeMask(argv[1]);

  std::vector<std::pair<size_t, size_t>> slopes = {
      {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

  size_t product = 1;
  for (auto [dx, dy] : slopes) {
    auto trees = CountTreesOnSlope(mask, dx, dy);
    printf("Encountered %zu trees on slope (%zu, %zu).\n", trees, dx, dy);
    product *= trees;
  }

  printf("Product of the above counts is: %zu.\n", product);
  return 0;
}
