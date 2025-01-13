#include "wlp4ScanningDFA.h"

bool Wlp4ScanningDFA::isChar(std::string s) {
    return s.length() == 1;
}
bool Wlp4ScanningDFA::isInRange(std::string s) {
    return s.length() == 3 && s[1] == '-';
}

void Wlp4ScanningDFA::initAlphabet(std::istringstream &dfa) {
    std::string s;
    std::getline(dfa, s); // Alphabet section (skip header)
    // Read characters or ranges separated by whitespace
    alphabet.emplace_back(' ');
    while(dfa >> s) {
        if (s == STATES) { 
            break; 
        } else {
            if (isChar(s)) {
                //// Variable 's[0]' is an alphabet symbol
                alphabet.emplace_back(s[0]);
            } else if (isInRange(s)) {
                for(char c = s[0]; c <= s[2]; ++c) {
                    //// Variable 'c' is an alphabet symbol
                    alphabet.emplace_back(c);
                }
            } 
        }
    }
}

void Wlp4ScanningDFA::initStates(std::istringstream &dfa) {
    std::string s;
    std::getline(dfa, s); // States section (skip header)
    // Read states separated by whitespace
    while(dfa >> s) {
        if (s == TRANSITIONS) { 
            break; 
        } else {
            static bool initial = true;
            bool accepting = false;
            if (s.back() == '!' && !isChar(s)) {
                accepting = true;
                s.pop_back();
            }
            //// Variable 's' contains the name of a state
            if (initial) {
                //// The state is initial
                if (accepting) {
                    states.emplace_back(std::make_pair(s, ACCEPTING));
                } else {
                    states.emplace_back(std::make_pair(s, INITIAL));
                }
                initial = false;
                continue;
            }
            if (accepting) {
                //// The state is accepting
                states.emplace_back(std::make_pair(s, ACCEPTING));
                continue;
            }
            states.emplace_back(std::make_pair(s, NONACCEPTING));
        }
    }
}

void Wlp4ScanningDFA::initTransitions(std::istringstream &dfa) {
    std::string s;
    std::getline(dfa, s); // Transitions section (skip header)
    // Read transitions line-by-line
    while(std::getline(dfa, s)) {
        std::string fromState, symbols, toState;
        std::istringstream line(s);
        std::vector<std::string> lineVec;
        while(line >> s) {
            lineVec.push_back(s);
        }
        fromState = lineVec.front();
        toState = lineVec.back();
        for(int i = 1; i < lineVec.size()-1; ++i) {
            std::string s = lineVec[i];
            if (isChar(s)) {
                symbols += s;
            } else if (isInRange(s)) {
                for(char c = s[0]; c <= s[2]; ++c) {
                    symbols += c;
                }
            }
        }
        for ( char c : symbols ) {
            //// There is a transition from 'fromState' to 'toState' on 'c'
            transitions.insert(std::make_pair(std::make_pair(fromState, c), toState));
        }
    }
}

Wlp4ScanningDFA::Wlp4ScanningDFA() {}

void Wlp4ScanningDFA::init() {
    std::istringstream dfa {wlp4TokenDFA};
    initAlphabet(dfa);
    initStates(dfa);
    initTransitions(dfa);
}

bool Wlp4ScanningDFA::findTransition(std::string &currentState, char transitionLetter) {
    try {
        currentState = transitions.at(std::make_pair(currentState, transitionLetter));
        return true;
    } catch (const std::out_of_range& e) {
        return false;
    }
}

int Wlp4ScanningDFA::findState(std::string currentState) {
    auto index = std::distance(states.begin(), std::find_if(states.begin(), states.end(), [&](const std::pair<std::string, stateType>& pair) { return pair.first == currentState; }));
    return index;
}

std::pair<std::string, stateType> Wlp4ScanningDFA::getState(int index) {
    return states.at(index);
}
