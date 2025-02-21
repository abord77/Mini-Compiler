#ifndef __BRANCHINSTRUCTION_H__
#define __BRANCHINSTRUCTION_H__
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map> 
#include "instruction.h"
#include "token.h"

class BranchInstruction : public Instruction {
    std::vector<Token> arguments;
    public:
        BranchInstruction(std::string instruction, int instructionNumber, std::vector<Token> arguments);
        bool verify(const std::unordered_map<std::string, uint32_t> &labelMap) override;
};

#endif
