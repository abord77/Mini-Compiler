#ifndef __WLP4CFG_H__
#define __WLP4CFG_H__
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "wlp4Data.h"

class Wlp4CFG {
    const std::string CFG         = ".CFG";
    const std::string DERIVATION  = ".DERIVATION";
    const std::string TRANSITIONS = ".TRANSITIONS";
    const std::string INPUT       = ".INPUT";
    const std::string ACTIONS     = ".ACTIONS";
    const std::string REDUCTIONS  = ".REDUCTIONS";
    const std::string END         = ".END";
    const std::string EMPTY       = ".EMPTY";

    std::vector<std::pair<std::string, std::vector<std::string>>> productions;
    std::map<std::pair<int, std::string>, int> transitions;
    std::map<std::pair<int, std::string>, int> reductions;
    Wlp4Data cfgDefinition;

    void initProductions(std::istringstream &dfa);
    void initTransitions(std::istringstream &dfa);
    void initReductions(std::istringstream &dfa);

    public:
        Wlp4CFG();
        void init();
        std::pair<std::string, std::vector<std::string>> getProduction(int index);
        int getTransition(int state, std::string productionLHS);
        int getReduction(int state, std::string nodeKind);
};

#endif
