#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__
#include <fstream>
#include <iostream>
#include "mipsMaximalMunch.h"
#include "mipsSemanticAnalysis.h"
#include "mipsSynthesis.h"

class Assembler {
    MipsSimplifiedMaximalMunch smm{};
    MipsSemanticAnalysis semanticAnalysis{};
    MipsSynthesis machineCodeGen{};

    public:
        Assembler();
        void assemble();
};

#endif
