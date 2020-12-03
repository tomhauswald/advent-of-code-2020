#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

struct DbEntry {
  char password[256];
  char character;
  size_t lo, hi;
};

std::vector<DbEntry> GetDbEntries(char const *path) {
  std::vector<DbEntry> entries;
  std::string line;

  auto stream = std::ifstream(path);
  while (std::getline(stream, line)) {
    DbEntry entry;
    sscanf(line.c_str(), "%zu-%zu %c: %s", &entry.lo, &entry.hi,
           &entry.character, &entry.password[0]);
    entries.push_back(entry);
  }

  return entries;
}

size_t CountCharOccurrences(char const *str, char c) {
  size_t count = 0;
  while (*str++)
    count += (*(str - 1) == c);
  return count;
}

enum class Policy : uint8_t { kMinMaxOcc, kPositional };

bool IsDbEntryValid(DbEntry const &entry, Policy policy) {
  switch (policy) {

  case Policy::kMinMaxOcc: {
    auto occ = CountCharOccurrences(entry.password, entry.character);
    return occ >= entry.lo && occ <= entry.hi;
  }

  case Policy::kPositional: {
    return ((entry.password[entry.lo - 1] == entry.character) ^
            (entry.password[entry.hi - 1] == entry.character));
  }

  default:
    return false;
  }
}

int main(int argc, char const **argv) {
  assert(argc == 2);
  auto entries = GetDbEntries(argv[1]);

  for (auto [name, policy] : {std::pair{"old", Policy::kMinMaxOcc},
                              std::pair{"new", Policy::kPositional}}) {
    printf("According to the %s policy, %zu passwords are valid.\n", name,
           std::count_if(std::begin(entries), std::end(entries),
                         [policy=policy](auto const &entry) {
                           return IsDbEntryValid(entry, policy);
                         }));
  }

  return 0;
}
