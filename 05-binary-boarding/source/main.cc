#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> ReadLinesFromFile(char const *path) {
  std::vector<std::string> lines;
  std::ifstream stream(path);
  std::string line;
  while (std::getline(stream, line))
    lines.push_back(line);
  return lines;
}

size_t ParseSeatId(std::string const &ticket) {
  assert(ticket.size() == 10);
  std::bitset<10> id;
  for (size_t i = 0; i < 10; ++i)
    id[9 - i] = (ticket.at(i) == 'B' || ticket.at(i) == 'R');
  return static_cast<size_t>(id.to_ulong());
}

int main(int argc, char const **argv) {
  assert(argc == 2);

  auto tickets = ReadLinesFromFile(argv[1]);
  std::vector<size_t> seat_ids;
  std::transform(tickets.begin(), tickets.end(), std::back_inserter(seat_ids),
                 ParseSeatId);
  std::sort(seat_ids.begin(), seat_ids.end());
  printf("Highest seat id is %zu.\n", seat_ids.back());

  for (size_t i = 0; i < seat_ids.size() - 1; ++i) {
    if (seat_ids[i + 1] == seat_ids[i] + 2) {
      printf("Your seat id is %zu.\n", seat_ids[i] + 1);
      break;
    }
  }

  return 0;
}
