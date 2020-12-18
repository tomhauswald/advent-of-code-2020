#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <set>

std::vector<std::string> ReadLinesFromFile(char const *path) {
  std::vector<std::string> lines;
  std::ifstream stream(path);
  std::string line;
  while (std::getline(stream, line))
    lines.push_back(line);
  return lines;
}

template<size_t Preamble_Len>
std::vector<size_t> FindSumsOfPreviousPairs(std::vector<size_t> const& numbers) {
  
  std::vector<size_t> indices;
  assert(numbers.size() > Preamble_Len && "Preamble length exceeds number list.");

  for(size_t curr = Preamble_Len; curr < numbers.size(); ++curr) {
    
    const auto target_sum = numbers.at(curr);
    const auto range_end = numbers.begin() + curr;
    const auto range_start = range_end - Preamble_Len;

    bool found = false;
    for(size_t lookback = 1; !found && lookback <= Preamble_Len; ++lookback) {
     
      const auto curr_preamble_value = numbers.at(curr - lookback);
      
      found |= (range_end != std::find(
        range_start, range_end,
        target_sum - curr_preamble_value));
    }

    if(!found) indices.push_back(curr);
  }

  return indices;
}

int main(int argc, char const **argv) {
  assert(argc == 2);

  const auto lines = ReadLinesFromFile(argv[1]);
  const auto len = lines.size();

  std::vector<size_t> numbers(len);
  for(size_t i=0; i<len; ++i)
    numbers[i] = std::strtoull(lines.at(i).c_str(), nullptr, 10);
  
  constexpr size_t preamble_len = 25;
  const auto indices = FindSumsOfPreviousPairs<preamble_len>(numbers);
  assert(indices.size() && "No match found.");

  bool contiguous = false;
  size_t start, len = 0;
  for(size_t i=0; i<indices.size(); ++i) {
    if((i + len + 1 < indices.size()) 
        && indices.at(i + len + 1) == indices.at(i + len) + 1) {
      contiguous = true;
      ++len;
  }

  return 0;
}
