#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

// Returns sorted list of numbers.
std::vector<size_t> GetReportNumbers(char const *path) {
  std::vector<size_t> numbers;
  std::string line;
  auto stream = std::ifstream(path);
  while (std::getline(stream, line))
    numbers.push_back(std::strtoull(line.c_str(), nullptr, 10));
  std::sort(std::begin(numbers), std::end(numbers));
  return numbers;
}

std::optional<std::tuple<size_t, size_t>>
FindTwoSummands(std::vector<size_t> const &numbers, const size_t wanted_sum) {
  size_t lo, hi, sum;
  size_t lower_index = 0;
  size_t upper_index = numbers.size() - 1;

  while (lower_index < upper_index) {
    lo = numbers[lower_index];
    hi = numbers[upper_index];
    sum = lo + hi;
    if (sum < wanted_sum)
      ++lower_index;
    else if (sum > wanted_sum)
      --upper_index;
    else
      return {{lo, hi}};
  }

  return std::nullopt;
}

std::optional<std::tuple<size_t, size_t, size_t>>
FindThreeSummands(std::vector<size_t> const &numbers, const size_t wanted_sum) {
  for (size_t index = 0; index < numbers.size(); ++index) {
    auto lo = numbers[index];
    if (auto others = FindTwoSummands(numbers, wanted_sum - lo); others) {
      auto [mid, hi] = others.value();
      return {{lo, mid, hi}};
    }
  }

  return std::nullopt;
}

int main(int argc, char const **argv) {
  assert(argc == 2);
  auto numbers = GetReportNumbers(argv[1]);

  if (auto summands = FindTwoSummands(numbers, 2020); summands) {
    auto [lo, hi] = summands.value();
    printf("%zu + %zu = %zu\n", lo, hi, lo + hi);
    printf("%zu * %zu = %zu\n", lo, hi, lo * hi);
  } else {
    printf("No two report numbers sum up to 2020.\n");
  }

  if (auto summands = FindThreeSummands(numbers, 2020); summands) {
    auto [lo, mid, hi] = summands.value();
    printf("%zu + %zu + %zu = %zu\n", lo, mid, hi, lo + mid + hi);
    printf("%zu * %zu * %zu = %zu\n", lo, mid, hi, lo * mid * hi);
  } else {
    printf("No three report numbers sum up to 2020.\n");
  }

  return 0;
}
