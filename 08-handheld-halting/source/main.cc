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

enum class Opcode : uint8_t {
  kNop, 
  kAcc, 
  kJmp 
};

struct Instruction {
  Opcode op;
  ssize_t arg;
};

using Program = std::vector<Instruction>;

Instruction Decode(std::string const& line) {
  auto name = line.substr(0, line.find(" "));
  auto arg = atoll(line.substr(line.find(" ") + 1).c_str());
   
  return Instruction { 
    .op = ((name == "nop") ? (Opcode::kNop) 
                           : ((name == "acc") ? (Opcode::kAcc) 
                                              : (Opcode::kJmp))),
    .arg = arg
  };
}

std::pair<bool, ssize_t> Execute(Program const& program) {
  std::set<size_t> visited_addresses;
  
  size_t pc = 0;
  ssize_t acc = 0;

  while(pc < program.size()) {
    
    auto [_, unique] = visited_addresses.insert(pc);
    if(!unique) return { false, acc };

    auto const& instr = program.at(pc);
    switch(instr.op) {
      
      case Opcode::kAcc:
        acc += instr.arg;
      case Opcode::kNop:
        ++pc;
      break;
      
      case Opcode::kJmp:
        pc += instr.arg;
      break;
    }
  }

  return { true, acc };
}

int main(int argc, char const **argv) {
  assert(argc == 2);

  auto lines = ReadLinesFromFile(argv[1]);
  
  Program program;
  std::transform(lines.begin(), lines.end(), std::back_inserter(program), Decode);
  
  auto [terminated, acc] = Execute(program);
  assert(!terminated && "Expected infinite loop.");
  printf("Program contains infinite loop.\n"); 
  printf("Value of accumulator before repeated instruction is %zu.\n", acc);

  for(auto& instr : program) {
    
    Opcode original_op = instr.op;
    if(instr.op == Opcode::kNop) {
      instr.op = Opcode::kJmp;
    } else if(instr.op == Opcode::kJmp) {
      instr.op = Opcode::kNop;
    }
    
    auto [terminated, acc] = Execute(program);
    if(terminated) {
      printf("Fixed infinite loop => program terminated.\n");
      printf("Final value of accumulator is %zu.\n", acc);
      return 0;
    } else {
      instr.op = original_op;
    }
  }

  assert(false && "No fix for infinite loop found.");
  return 1;
}
