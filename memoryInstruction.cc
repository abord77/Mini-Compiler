#include "memoryInstruction.h"

MemoryInstruction::MemoryInstruction(std::string instruction, int instructionNumber, std::vector<Token> arguments)
    : Instruction{instruction, instructionNumber}, arguments{std::move(arguments)} {}

bool MemoryInstruction::verify(const std::unordered_map<std::string, uint32_t> &labelMap) {
    if (arguments.size() != 6) return false;

    for (int i = 0; i < arguments.size(); ++i) {
        if (i == 1) {
            if (arguments[i].getTokenType() != COMMA) return false;
        } else if (i == 3) {
            if (arguments[i].getTokenType() != LPAREN) return false;
        } else if (i == 5) {
            if (arguments[i].getTokenType() != RPAREN) return false;
        } else if (i == 0 || i == 4) {
            if (arguments[i].getTokenType() != REG) return false;
            int registerValue = stoi(arguments[i].getLexeme().substr(arguments[i].getLexeme().find('$') + 1));
            if (registerValue < 0 && registerValue > 31) return false;
            parameters[parameterCount] = registerValue;
            parameterCount++;
        } else {
            if (arguments[i].getTokenType() == DEC) {
                int decimalValue = stoi(arguments[i].getLexeme());
                if (decimalValue < -32768 || decimalValue > 32767) return false;
                parameters[parameterCount] = decimalValue;
            } else if (arguments[i].getTokenType() == HEXINT) {
                int decimalValue;
                std::stringstream input {arguments[i].getLexeme()};
                input >> std::hex >> decimalValue >> std::dec;
                if (decimalValue < -32768 || decimalValue > 32767) return false;
                parameters[parameterCount] = decimalValue;
            } else {
                return false;
            }
            parameterCount++;
        }
    }
    return true;
}
