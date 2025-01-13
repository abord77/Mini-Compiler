#include "wlp4CFG.h"

void Wlp4CFG::initProductions(std::istringstream &dfa) {
    std::string s;
    std::getline(dfa, s); // Productions section (skip header)
    // Read characters or ranges separated by whitespace
    while(std::getline(dfa, s)) {
        if (s == TRANSITIONS) { 
            break; 
        } else {
            std::istringstream productionWords{s};
            std::vector<std::string> rhs;
            std::string lhsRule;
            std::string rhsRule;
            productionWords >> lhsRule;
            while (productionWords >> rhsRule) {
                rhs.emplace_back(rhsRule);
            }
            productions.emplace_back(std::make_pair(lhsRule, rhs));
        }
    }
}

void Wlp4CFG::initTransitions(std::istringstream &dfa) {
    std::string s;
    // Transitions section header is already skipped
    // Read transitions line-by-line
    while(std::getline(dfa, s)) {
        if (s == REDUCTIONS) { 
            break; 
        } else {
            std::istringstream transitionStr{s};
            int currentTransition;
            std::string currentSymbol;
            int nextTransition;
            transitionStr >> currentTransition >> currentSymbol >> nextTransition;
            transitions.insert(std::make_pair(std::make_pair(currentTransition, currentSymbol), nextTransition));
        }
    }
}

void Wlp4CFG::initReductions(std::istringstream &dfa) {
    std::string s;
    // Reductions section header is already skipped
    // Read reductions line-by-line
    while(std::getline(dfa, s)) {
        if (s == END) { 
            break; 
        } else {
            std::istringstream reductionStr{s};
            int currentState;
            std::string nextSymbol;
            int reductionRule;
            reductionStr >> currentState >> reductionRule >> nextSymbol;
            reductions.insert(std::make_pair(std::make_pair(currentState, nextSymbol), reductionRule));
        }
    }
}

Wlp4CFG::Wlp4CFG() {}

void Wlp4CFG::init() {
    std::istringstream dfa{cfgDefinition.WLP4_COMBINED};
    initProductions(dfa);
    initTransitions(dfa);
    initReductions(dfa);
}

std::pair<std::string, std::vector<std::string>> Wlp4CFG::getProduction(int index) {
    return productions.at(index);
}

int Wlp4CFG::getTransition(int state, std::string nextSym) {
    try {
        return transitions.at(std::make_pair(state, nextSym));
    } catch (const std::out_of_range &e) {
        return -1;
    }
}

int Wlp4CFG::getReduction(int state, std::string nodeKind) {
    try {
        return reductions.at(std::make_pair(state, nodeKind));
    } catch (const std::out_of_range &e) {
        return -1;
    }
}
