#include "branchInstruction.h"

BranchInstruction::BranchInstruction(std::string instruction, int instructionNumber, std::vector<Token> arguments)
    : Instruction{instruction, instructionNumber}, arguments{std::move(arguments)} {}

bool BranchInstruction::verify(const std::unordered_map<std::string, uint32_t> &labelMap) {
    if (arguments.size() != 5) return false;

    for (int i = 0; i < arguments.size(); ++i) {
        if (i == 1 || i == 3) {
            if (arguments[i].getTokenType() != COMMA) return false;
        } else if (i == 0 || i == 2) {
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
            } else if (arguments[i].getTokenType() == ID) {
                auto pos = labelMap.find(arguments[i].getLexeme()); 
                if (pos == labelMap.end()) return false;
                int labelValue = pos->second;
                int decimalValue = (labelValue - instructionNumber - 4) / 4;
                if (decimalValue < -32768 || decimalValue > 32767) return false;
                u_int16_t unsignedDecimalValue = decimalValue;
                parameters[parameterCount] = unsignedDecimalValue;
            } else {
                return false;
            }
            parameterCount++;
        }
    }
    return true;
}
