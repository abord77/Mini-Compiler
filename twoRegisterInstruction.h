#ifndef __TWOREGISTERINSTRUCTION_H__
#define __TWOREGISTERINSTRUCTION_H__
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map> 
#include "instruction.h"
#include "token.h"

class TwoRegistersInstruction : public Instruction {
    std::vector<Token> arguments;
    public:
        TwoRegistersInstruction(std::string instruction, int instructionNumber, std::vector<Token> arguments);
        bool verify(const std::unordered_map<std::string, uint32_t> &labelMap) override;
};

#endif
