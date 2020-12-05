#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> ReadLinesFromFile(char const *path) {
  std::vector<std::string> lines;
  std::ifstream stream(path);
  std::string line;
  while (std::getline(stream, line))
    lines.push_back(line);
  return lines;
}

std::vector<std::string> SplitString(std::string const &str, char const *sep) {
  std::vector<std::string> parts;
  size_t curr = 0, next = 0;
  while ((next = str.find(sep, curr)) != std::string::npos) {
    parts.push_back(str.substr(curr, next - curr));
    curr = next + 1;
  }
  parts.push_back(str.substr(curr));
  return parts;
}

using Passport = std::unordered_map<std::string, std::string>;

void AddPassportData(Passport &passport, std::string const &kv_str) {
  char kbuf[256], vbuf[256];
  assert(sscanf(kv_str.c_str(), "%[^:]:%s", kbuf, vbuf) == 2);
  passport[kbuf] = vbuf;
}

bool BirthYearIsValid(std::string const &value) {
  auto year = std::atoi(value.c_str());
  return 1920 <= year && year <= 2002;
}

bool IssueYearIsValid(std::string const &value) {
  auto year = std::atoi(value.c_str());
  return 2010 <= year && year <= 2020;
}

bool ExpirationYearIsValid(std::string const &value) {
  auto year = std::atoi(value.c_str());
  return 2020 <= year && year <= 2030;
}

bool HeightIsValid(std::string const &value) {
  if (value.size() < 4)
    return false;
  auto unit = value.substr(value.size() - 2);
  auto height = atoi(value.substr(0, value.size() - 2).c_str());
  return ((unit == "cm" && 150 <= height && height <= 193) ||
          (unit == "in" && 59 <= height && height <= 76));
}

bool HairColorIsValid(std::string const &value) {
  if (value.at(0) != '#')
    return false;
  for (auto c : value.substr(1))
    if ((c < '0' || c > '9') && (c < 'a' || c > 'f'))
      return false;
  return true;
}

bool EyeColorIsValid(std::string const &value) {
  static constexpr auto colors = {"amb", "blu", "brn", "gry",
                                  "grn", "hzl", "oth"};
  return (std::find(colors.begin(), colors.end(), value) != colors.end());
}

bool PassportIdIsValid(std::string const &value) {
  if (value.size() != 9)
    return false;
  for (auto c : value)
    if (c < '0' && c > '9')
      return false;
  return true;
}

std::optional<std::string> GetPassportField(Passport const &passport,
                                            std::string const &field) {
  auto it = passport.find(field);
  if (it == passport.end())
    return std::nullopt;
  return it->second;
}

bool PassportIsValid(Passport const &passport) {
  if (passport.size() < 7)
    return false;

  auto byr = GetPassportField(passport, "byr");
  if (!byr || !BirthYearIsValid(byr.value()))
    return false;

  auto iyr = GetPassportField(passport, "iyr");
  if (!iyr || !IssueYearIsValid(iyr.value()))
    return false;

  auto eyr = GetPassportField(passport, "eyr");
  if (!eyr || !ExpirationYearIsValid(eyr.value()))
    return false;

  auto hgt = GetPassportField(passport, "hgt");
  if (!hgt || !HeightIsValid(hgt.value()))
    return false;

  auto hcl = GetPassportField(passport, "hcl");
  if (!hcl || !HairColorIsValid(hcl.value()))
    return false;

  auto ecl = GetPassportField(passport, "ecl");
  if (!ecl || !EyeColorIsValid(ecl.value()))
    return false;

  auto pid = GetPassportField(passport, "pid");
  if (!pid || !PassportIdIsValid(pid.value()))
    return false;

  return true;
}

int main(int argc, char const **argv) {
  assert(argc == 2);
  auto lines = ReadLinesFromFile(argv[1]);

  std::vector<Passport> passports = {Passport{}};
  for (auto const &line : lines) {
    if (line.empty()) {
      passports.push_back({});
    } else {
      for (auto const &kv_str : SplitString(line, " ")) {
        AddPassportData(passports.back(), kv_str);
      }
    }
  }

  printf("%zu passports are valid.\n",
         std::count_if(passports.begin(), passports.end(), PassportIsValid));

  return 0;
}
