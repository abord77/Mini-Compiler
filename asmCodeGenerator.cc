#include "asmCodeGenerator.h"

AsmCodeGenerator::AsmCodeGenerator() {}

void AsmCodeGenerator::compile() {
    smm.parse();
    machineCodeGen.generate();
}
