#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using Question = char;
using Person = std::string;
using Group = std::vector<Person>;

std::vector<Group> ParseGroups(char const *path) {
  std::vector<Group> groups(1);
  
  std::ifstream stream(path);
  std::string line;
  while (std::getline(stream, line)) {
    if(line.empty())
      groups.push_back(Group{});
    else 
      groups.back().push_back(static_cast<Person>(line));
  }

  return groups;
}

int main(int argc, char const **argv) {
  assert(argc == 2);

  auto groups = ParseGroups(argv[1]);
  size_t sum_of_answered_by_any = 0;
  size_t sum_of_answered_by_all = 0;

  for(auto const& group : groups) {
    auto answered_by_any = std::bitset<26>{};
    auto answered_by_all = std::bitset<26>{}.set();
    for(auto const& person : group) {
      std::bitset<26> answered{};
      for(Question q : person) {
        answered[q - 'a'] = 1;
      }
      answered_by_any |= answered;
      answered_by_all &= answered;
    }
    sum_of_answered_by_any += answered_by_any.count();
    sum_of_answered_by_all += answered_by_all.count();
  }

  printf("The sum over all groups of...\n");
  printf("  ...the number of questions answered by anyone inside it is %zu.\n", sum_of_answered_by_any);
  printf("  ...the number of questions answered by everyone inside it is %zu.\n", sum_of_answered_by_all);

  return 0;
}
