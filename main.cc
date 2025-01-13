#include "assembler.h"
#include "asmCodeGenerator.h"


int main(int argc, char * argv[]) {
    Assembler assembler{};
    AsmCodeGenerator codeGenerator{};

    codeGenerator.compile();
    assembler.assemble();
}

