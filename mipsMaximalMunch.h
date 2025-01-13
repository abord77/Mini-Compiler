#ifndef __MIPSMAXIMALMUNCH_H__
#define __MIPSMAXIMALMUNCH_H__
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include "mipsScanningDFA.h"

class MipsSimplifiedMaximalMunch {
    int currentTokenStart;
    int currentTokenEnd;
    bool encounteredError;
    MipsScanningDFA mipsDFA;

    public:
        MipsSimplifiedMaximalMunch();
        void parse();
};

#endif
