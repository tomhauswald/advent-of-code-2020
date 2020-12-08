#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> ReadLinesFromFile(char const *path) {
  std::vector<std::string> lines;
  std::ifstream stream(path);
  std::string line;
  while (std::getline(stream, line))
    lines.push_back(line);
  return lines;
}

std::vector<std::string> SplitString(std::string const& str, std::string const& sep) {
  std::vector<std::string> parts;
  size_t curr=0, next=0;
  while((next = str.find(sep, curr)) != std::string::npos) {
    parts.push_back(str.substr(curr, next-curr));
    curr = next + sep.size();
  }
  parts.push_back(str.substr(curr));
  return parts;
}
  
using Rulebook = std::unordered_map<
  std::string, 
  std::unordered_map<std::string, size_t>
>;

void AddRule(Rulebook& book, std::string const& rule) {
  auto parts = SplitString(rule, " bags contain ");
  auto const& parent_color = parts.at(0);
  
  for(auto const& child : SplitString(parts.at(1), ", ")) {
    auto first_space_pos = child.find(" ");
    auto last_space_pos = child.find_last_of(" ");
    
    auto child_amount = static_cast<size_t>(
      atoll(child.substr(0, first_space_pos).c_str())
    );
    
    auto child_color = child.substr(
      first_space_pos + 1, 
      last_space_pos - (first_space_pos + 1)
    );
    
    book[parent_color][child_color] = child_amount;
  }
}

bool CanColorHoldColor(
  Rulebook const& rules,
  std::string const& parent,
  std::string const& child) {

  if(!rules.count(parent))
    return false;
  
  if(rules.at(parent).count(child))
    return true;

  // DFS
  for(auto const& [direct_child, _] : rules.at(parent))
    if(CanColorHoldColor(rules, direct_child, child))
      return true;
  
  return false;
}

size_t GetNumberOfChildBags(
  Rulebook const& rules,
  std::string const& parent) {

  if(!rules.count(parent))
    return 0;
 
  // DFS
  size_t subsum = 0;
  for(auto const& [child, amount] : rules.at(parent))
    subsum += amount * (1 + GetNumberOfChildBags(rules, child));
  
  return subsum;
}

int main(int argc, char const **argv) {
  assert(argc == 2);
  
  Rulebook rules;
  for(auto const& line : ReadLinesFromFile(argv[1])) 
    AddRule(rules, line);

  auto count = std::count_if(
    rules.begin(), 
    rules.end(), 
    [&rules](auto const& pair) {
      return CanColorHoldColor(rules, pair.first, "shiny gold");
    }
  );

  printf("%zu\n", rules.at("shiny gold").size());
  printf("There are %zu bag colors that can hold shiny gold bags.\n", count);
  printf("A single shiny gold bag contains %zu bags.\n", GetNumberOfChildBags(rules, "shiny gold"));
  return 0;
}
