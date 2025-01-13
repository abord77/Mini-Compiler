#include "mipsSemanticAnalysis.h"

void MipsSemanticAnalysis::readTokens(std::istream &in) {
    std::string line;
    while(std::getline(in, line)) {
        std::string tokenType;
        std::string lexeme;

        std::stringstream lineParser(line);
        lineParser >> tokenType;
        if(tokenType != "NEWLINE") {
            lineParser >> lexeme;
            tokens.push_back(Token(Token::stringToTokenType(tokenType), lexeme));
        } else if (tokenType == "NEWLINE") {
            tokens.push_back(Token(NONE, "NEWLINE"));
        }
    }
}

MipsSemanticAnalysis::MipsSemanticAnalysis() {}

void MipsSemanticAnalysis::semanticParse() {
    std::ofstream mipsParameters;
    mipsParameters.open("!6.mipsParameters.txt", std::fstream::out);
    std::ifstream tokenization;
    tokenization.open("!5.mipsTokens.txt", std::fstream::in);
    readTokens(tokenization);
    int instructionCount = 0;
    bool errorEncountered = false;

    labelMap.insert(std::make_pair("print", 0));
    labelMap.insert(std::make_pair("init", 4));
    labelMap.insert(std::make_pair("new", 8));
    labelMap.insert(std::make_pair("delete", 16));

    for (int i = 8; i < tokens.size(); ++i) {
        if (errorEncountered) {
            break;
        } else if (tokens[i].getTokenType() == NONE) {
            continue;
        } else if (tokens[i].getTokenType() == LABEL) {
            std::string label = tokens[i].getLexeme().substr(0, tokens[i].getLexeme().find(':'));
            labelMap.insert(std::make_pair(label, instructionCount * 4));
        } else if (tokens[i].getTokenType() == ID || tokens[i].getTokenType() == DIRECTIVE) {
            int instructionType;
            auto it = INSTRUCTION_SET.find(tokens[i].getLexeme());
            if (it != INSTRUCTION_SET.end()) {
                instructionType = it->second;
            } else {
                std::cerr << "ERROR: Invalid instruction type (maybe used a label when not supposed to) of \"" << tokens[i].getLexeme() << "\"" << std::endl;
                break;
            }

            std::vector<Token> arguments;
            int j = 0;
            switch(instructionType) {
                case 1: // opcode rrr
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG || tokens[i + j + 1].getTokenType() == COMMA) {
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new ThreeRegistersInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 2: // opcode rr
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG || tokens[i + j + 1].getTokenType() == COMMA) {
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new TwoRegistersInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 3: // opcode r (not lis)
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG) {
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new OneRegisterInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 4: // lis + directive
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG) {
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new OneRegisterInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 5: // opcode branch
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG || tokens[i + j + 1].getTokenType() == COMMA || tokens[i + j + 1].getTokenType() == DEC || tokens[i + j + 1].getTokenType() == HEXINT || tokens[i + j + 1].getTokenType() == ID) {
                            if (arguments.size() == 5) {
                                break;
                            }
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    
                    instructions.emplace_back(new BranchInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 6: // opcode memory
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == REG || tokens[i + j + 1].getTokenType() == COMMA || tokens[i + j + 1].getTokenType() == LPAREN || tokens[i + j + 1].getTokenType() == RPAREN || tokens[i + j + 1].getTokenType() == DEC || tokens[i + j + 1].getTokenType() == HEXINT) {
                            if (arguments.size() == 6) {
                                break;
                            }
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new MemoryInstruction{tokens[i].getLexeme(), instructionCount, arguments});
                    i += j;
                    break;
                case 7: // .word directive
                    while (true) {
                        if (tokens[i + j + 1].getTokenType() == DEC || tokens[i + j + 1].getTokenType() == HEXINT || tokens[i + j + 1].getTokenType() == ID) {
                            if (arguments.size() == 1) {
                                break;
                            }
                            arguments.emplace_back(tokens[i + j + 1]);
                            j++;
                        } else {
                            break;
                        }
                    }
                    instructions.emplace_back(new Directive{"word", instructionCount, arguments});
                    i += j;
                    break;
            }

            instructionCount++;
        } else {
            std::cerr << "ERROR: Not a label or ID or .word directive, it is a " << Token::tokenTypeToString(tokens[i].getTokenType()) << std::endl;
            break;
        }
    }
    for (int i = 0; i < instructions.size(); ++i) {
        if (!instructions[i].get()->verify(labelMap)) {
            std::cerr << "ERROR: Instruction has faulty arguments" << std::endl;
            std::cerr << instructions[i].get() << std::endl;
            break;
        }
        mipsParameters << instructions[i].get() << std::endl;
    }
}
