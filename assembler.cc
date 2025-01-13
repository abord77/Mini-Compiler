#include "assembler.h"

Assembler::Assembler() {}

void Assembler::assemble() { // https://student.cs.uwaterloo.ca/~cs241/webtools/
    smm.parse();
    semanticAnalysis.semanticParse();
    machineCodeGen.generate();
}
