#ifndef __WLP4MAXIMALMUNCH_H__
#define __WLP4MAXIMALMUNCH_H__
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include "wlp4ScanningDFA.h"

class Wlp4SimplifiedMaximalMunch {
    int currentTokenStart;
    int currentTokenEnd;
    bool encounteredError;
    Wlp4ScanningDFA wlp4DFA;

    public:
        Wlp4SimplifiedMaximalMunch();
        void parse();
};

#endif
