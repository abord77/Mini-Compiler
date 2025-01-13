#ifndef __WLP4CODEGENERATOR_H__
#define __WLP4CODEGENERATOR_H__
#include "wlp4MaximalMunch.h"
#include "wlp4Synthesis.h"

class AsmCodeGenerator {
    Wlp4SimplifiedMaximalMunch smm{};
    Wlp4Synthesis machineCodeGen{};

    public:
        AsmCodeGenerator();
        void compile();
};

#endif
