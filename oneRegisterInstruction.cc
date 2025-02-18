#include "oneRegisterInstruction.h"

OneRegisterInstruction::OneRegisterInstruction(std::string instruction, int instructionNumber, std::vector<Token> arguments)
    : Instruction{instruction, instructionNumber}, arguments{std::move(arguments)} {}

bool OneRegisterInstruction::verify(const std::unordered_map<std::string, uint32_t> &labelMap) {
    if (arguments.size() != 1) return false;

    if (arguments[0].getTokenType() != REG) return false;
    int registerValue = stoi(arguments[0].getLexeme().substr(arguments[0].getLexeme().find('$') + 1));
    if (registerValue < 0 && registerValue > 31) return false;
    parameters[parameterCount] = registerValue;
    parameterCount++;

    return true;
}
