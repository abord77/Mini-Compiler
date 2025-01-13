#ifndef __MIPSSEMANTICANALYSIS_H__
#define __MIPSSEMANTICANALYSIS_H__
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map> 
#include <memory>
#include "threeRegisterInstruction.h"
#include "twoRegisterInstruction.h"
#include "oneRegisterInstruction.h"
#include "branchInstruction.h"
#include "memoryInstruction.h"
#include "directiveInstruction.h"
#include "token.h"

class MipsSemanticAnalysis {
    const std::map<std::string, int> INSTRUCTION_SET = {
        {"add",   1},
        {"sub",   1},
        {"div",   2},
        {"divu",  2},
        {"mult",  2},
        {"multu", 2},
        {"mfhi",  3},
        {"mflo",  3},
        {"slt",   1},
        {"sltu",  1},
        {"beq",   5},
        {"bne",   5},
        {"jalr",  3},
        {"jr",    3},
        {"lis",   4},
        {"lw",    6},
        {"sw",    6},
        {".word", 7}
    };

    std::vector<Token> tokens;
    std::unordered_map<std::string, uint32_t> labelMap;
    std::vector<std::unique_ptr<Instruction>> instructions;

    void readTokens(std::istream &in);
    public:
        MipsSemanticAnalysis();
        void semanticParse();
};

#endif
