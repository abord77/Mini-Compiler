#include "mipsMaximalMunch.h"

MipsSimplifiedMaximalMunch::MipsSimplifiedMaximalMunch()
    : currentTokenStart{0}, currentTokenEnd{0}, encounteredError{false} {
        mipsDFA.init();
}

void MipsSimplifiedMaximalMunch::parse() {
    std::ofstream tokenization;
    tokenization.open("!5.mipsTokens.txt", std::fstream::out);
    std::ifstream sourceCode;
    sourceCode.open("!4.mipsSourceCode.asm", std::fstream::in);
    
    std::string s;
    while(sourceCode >> s) {
        currentTokenStart = 0;
        currentTokenEnd = 0;
        encounteredError = false;
        for (int i = 0; i < s.length(); ++i) {
            std::string currentState = mipsDFA.getState(0).first; // set this to whatever the start state is defined as
            while (true) {
                if (s[currentTokenEnd] == ';') { // encountered a mips comment
                    getline(sourceCode, s);
                    encounteredError = true;
                    break;
                }

                if (mipsDFA.findTransition(currentState, s[currentTokenEnd])) {
                    currentTokenEnd++;
                    i++;
                } else {
                    int index = mipsDFA.findState(currentState);
                    auto state = mipsDFA.getState(index);
                    if (state.second == ACCEPTING) {
                        std::string stateName = state.first;
                        std::string token = stateName.substr(0, stateName.find('_'));
                        tokenization << token << " " << s.substr(currentTokenStart, currentTokenEnd - currentTokenStart) << std::endl;
                        currentTokenStart = currentTokenEnd;
                        i--;
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
        }
    }
    tokenization.close();
}
