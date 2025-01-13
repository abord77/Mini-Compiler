#include "wlp4MaximalMunch.h"

Wlp4SimplifiedMaximalMunch::Wlp4SimplifiedMaximalMunch()
    : currentTokenStart{0}, currentTokenEnd{0}, encounteredError{false} {
        wlp4DFA.init();
}

void Wlp4SimplifiedMaximalMunch::parse() {
    std::istream &in = std::cin;
    std::ofstream tokenization;
    tokenization.open("!1.wlp4Tokens.txt", std::fstream::out);
    std::string s;
    while(in >> s) {
    //// Variable 's' contains an input string for the DFA
        int currentTokenStart = 0;
        int currentTokenEnd = 0;
        bool encounteredError = false;
        bool commentBlock = false;

        for (int i = 0; i < s.length(); ++i) {
            std::string currentState = wlp4DFA.getState(0).first; // set this to whatever the start state is defined as
            while (true) {
                if (wlp4DFA.findTransition(currentState, s[currentTokenEnd])) {
                    currentTokenEnd++;
                    if (currentState == "COMMENT") {
                        commentBlock = true;
                        break;
                    }
                    i++;
                } else {
                    int index = wlp4DFA.findState(currentState);
                    auto state = wlp4DFA.getState(index);
                    if (state.second == ACCEPTING && state.first != "NUM_0" && state.first != "NUM_1" && state.first != "NUM_2") {
                        std::string stateName = state.first;
                        std::string token = stateName.substr(0, stateName.find('_'));
                        if (token == "seen") token = "ID";
                        tokenization << token << " " << s.substr(currentTokenStart, currentTokenEnd - currentTokenStart) << std::endl; // " " << currentTokenStart << currentTokenEnd <<
                        currentTokenStart = currentTokenEnd;
                        i--;
                    } else if (state.second == ACCEPTING && (state.first != "NUM_0" || state.first != "NUM_1" || state.first != "NUM_2")) {
                        double num = std::stod(s.substr(currentTokenStart, currentTokenEnd - currentTokenStart));
                        if (num > 2147483647) {
                            encounteredError = true; // we have read in a value greater than we can handle
                            std::cerr << "ERROR: Read token that is too large: " << s.substr(currentTokenStart, currentTokenEnd - currentTokenStart + 1) << std::endl;
                        } else {
                            std::string stateName = state.first;
                            std::string token = stateName.substr(0, stateName.find('_'));
                            tokenization << token << " " << s.substr(currentTokenStart, currentTokenEnd - currentTokenStart) << std::endl; 
                            currentTokenStart = currentTokenEnd;
                            i--;
                        }
                    } else {
                        encounteredError = true; // we are left in a nonaccepting state so quit
                        std::cerr << "ERROR: In a nonaccepting statement with token: " << s.substr(currentTokenStart, currentTokenEnd - currentTokenStart + 1) << std::endl;
                    }
                    break;
                }
            }
            if (encounteredError) {
                break;
            }

            if (commentBlock) {
                getline(in, s);
                commentBlock = false;
                break;
            }
        }
    }
    tokenization.close();
}
