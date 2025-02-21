#include "directiveInstruction.h"

Directive::Directive(std::string instruction, int instructionNumber, std::vector<Token> arguments)
    : Instruction{instruction, instructionNumber}, arguments{std::move(arguments)} {}

bool Directive::verify(const std::unordered_map<std::string, uint32_t> &labelMap) {
    if (arguments.size() != 1) return false;
    if (arguments[0].getTokenType() == DEC) {
        long decimalValue = stoi(arguments[0].getLexeme());
        if (decimalValue < -2147483648 || decimalValue > 4294967295) return false;
        parameters[parameterCount] = decimalValue;
    } else if (arguments[0].getTokenType() == HEXINT) {
        long decimalValue;
        std::stringstream input {arguments[0].getLexeme()};
        input >> std::hex >> decimalValue >> std::dec;
        if (decimalValue < -2147483648 || decimalValue > 4294967295) return false;
        parameters[parameterCount] = decimalValue;
    } else if (arguments[0].getTokenType() == ID) {
        auto pos = labelMap.find(arguments[0].getLexeme()); 
        if (pos == labelMap.end()) return false;
        u_int32_t  decimalValue = pos->second;
        if (decimalValue < -2147483648 || decimalValue > 4294967295) return false;
        parameters[parameterCount] = decimalValue;
    } else {
        return false;
    }
    parameterCount++;

    return true;
}
