#include "mipsSynthesis.h"

MipsSynthesis::MipsSynthesis(): 
    ASM_LINE_PATTERN{"^\\s*([a-z]+)\\s*((\\$)(\\d+)|((0x[0-9a-fA-F]*)|(\\d+)))\\s*(,\\s*(((\\$)?(\\d+)|0x[0-9a-fA-F]*)|((0x[0-9a-fA-F]*)|(-?\\d+))\\s*\\(\\s*(\\$)(\\d+)\\s*\\))?\\*?(,\\s*(((\\$)?(-?\\d+))|(0x[0-9a-fA-F]*)))?)?\\s*$"}, 
    EMPTY_LINE{"^\\s*(;.*)?$"} {}

void MipsSynthesis::formatError(const std::string & message) {
    std::cerr << "ERROR: " << message << std::endl;
}

bool MipsSynthesis::registerFormatInputCheck(uint32_t one, uint32_t two, uint32_t three, std::string instruction) {
    bool isValid = true;
    if (one > 31) {
        formatError("First parameter for " + instruction + " is not a valid register");
        isValid = false;
    }
    if (two > 31) {
        formatError("Second parameter for " + instruction + " is not a valid register");
        isValid = false;
    }
    if (three > 31) {
        formatError("Third parameter for " + instruction + " is not a valid register");
        isValid = false;
    }
    return isValid;
}

bool MipsSynthesis::immediateFormatInputCheck(uint32_t one, uint32_t two, uint32_t three, bool isBranch, std::string instruction) {
    bool isValid = true;
    if (one > 31) {
        formatError("First parameter for " + instruction + " is not a valid register");
        isValid = false;
    }
    if (isBranch) {
        if (two > 31) {
            formatError("Second parameter for " + instruction + " is not a valid register");
            isValid = false;
        }
    } else {
        if (three > 31) {
            formatError("Third parameter for " + instruction + " is not a valid register");
            isValid = false;
        }
    }
    return isValid;
}

std::string MipsSynthesis::decimalToBinaryRegister(uint32_t decimalVal) {
    int digits[5];
    std::string binary = "";
    for (int i = 4; i >= 0; --i) {
        if (decimalVal == 0) {
            digits[i] = 0;
        }
        digits[i] = decimalVal % 2;
        decimalVal /= 2;
    }
    for (int i = 0; i < 5; i++) {
        binary += std::to_string(digits[i]);
    }
    return binary;
}

std::string MipsSynthesis::decimalToBinaryImmediate(uint32_t decimalVal) {
    int digits[16];
    std::string binary = "";
    for (int i = 15; i >= 0; --i) {
        if (decimalVal == 0) {
            digits[i] = 0;
        }
        digits[i] = decimalVal % 2;
        decimalVal /= 2;
    }
    for (int i = 0; i < 16; i++) {
        binary += std::to_string(digits[i]);
    }
    return binary;
}

std::string MipsSynthesis::decimalToBinaryWord(uint32_t decimalVal) {
    int digits[32];
    std::string binary = "";
    for (int i = 31; i >= 0; --i) {
        if (decimalVal == 0) {
            digits[i] = 0;
        }
        digits[i] = decimalVal % 2;
        decimalVal /= 2;
    }
    for (int i = 0; i < 32; i++) {
        binary += std::to_string(digits[i]);
    }
    return binary;
}

int MipsSynthesis::binaryToDecimal(std::string binaryVal) {
    uint32_t decimal = 0;
    uint32_t base = 1;
    for (int i = binaryVal.length() - 1; i >= 0; --i) {
        int tmp = binaryVal[i] - 48;
        decimal += tmp * base;
        base *= 2;
    }
    return decimal;
}

bool MipsSynthesis::compileLine(uint32_t &          word,
                 const std::string & instruction,
                 uint32_t            one,
                 uint32_t            two,
                 uint32_t            three) {
    if (instruction == "add" && registerFormatInputCheck(one, two, three, instruction)) { 
        std::string binary = "000000" + decimalToBinaryRegister(two) + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + "00000100000";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "sub" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(two) + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + "00000100010";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "mult" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + "0000000000011000";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "multu" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + "0000000000011001";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "div" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + "0000000000011010";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "divu" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + "0000000000011011";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "mfhi" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "0000000000000000" + decimalToBinaryRegister(one) + "00000010000";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "mflo" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "0000000000000000" + decimalToBinaryRegister(one) + "00000010010";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "lis" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "0000000000000000" + decimalToBinaryRegister(one) + "00000010100";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "slt" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(two) + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + "00000101010";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "sltu" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(two) + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + "00000101011";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "jr" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + "000000000000000001000";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "jalr" && registerFormatInputCheck(one, two, three, instruction)) {
        std::string binary = "000000" + decimalToBinaryRegister(one) + "000000000000000001001";
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "beq" && immediateFormatInputCheck(one, two, three, true, instruction)) {
        std::string binary = "000100" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + decimalToBinaryImmediate(three);
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "bne" && immediateFormatInputCheck(one, two, three, true, instruction)) {
        std::string binary = "000101" + decimalToBinaryRegister(one) + decimalToBinaryRegister(two) + decimalToBinaryImmediate(three);
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "lw" && immediateFormatInputCheck(one, two, three, false, instruction)) {
        std::string binary = "100011" + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + decimalToBinaryImmediate(two);
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "sw" && immediateFormatInputCheck(one, two, three, false, instruction)) {
        std::string binary = "101011" + decimalToBinaryRegister(three) + decimalToBinaryRegister(one) + decimalToBinaryImmediate(two);
        word = binaryToDecimal(binary);
        return true;
    } else if (instruction == "word") {
        std::string binary = decimalToBinaryWord(one);
        word = binaryToDecimal(binary);
        return true;
    }

    return false; // this means an insturction is not recognized or had bad input in registers/immediate
}

uint32_t MipsSynthesis::stouint32(const std::string & s) {
    if (s == "0") {
        return 0;
    }

    if (s.length() >= 2 && s[0] == '0' && s[1] == 'x') {
        return std::stoi(s.substr(2), nullptr, 16);
    }

    if (s.length() >= 1 && s[0] == '0') {
        return std::stoi(s.substr(1), nullptr, 8);
    }

    return std::stoi(s);
}

void MipsSynthesis::generate() {
    std::ifstream parameters;
    parameters.open("!6.mipsParameters.txt", std::fstream::in);
    std::ofstream mipsMachineCode;
    mipsMachineCode.open("!compiled.mips", std::fstream::out);

    std::string line;
    while(std::getline(parameters, line)) {
        std::stringstream instruction{line};
        uint32_t binary = 0;
        std::string opcode;
        u_int32_t param1, param2, param3;
        instruction >> opcode >> param1 >> param2 >> param3;
        bool compiled = compileLine(binary, opcode, param1, param2, param3);
        if (compiled) {
            mipsMachineCode << (char)((binary >> 24) & 0xFF)
                            << (char)((binary >> 16) & 0xFF)
                            << (char)((binary >>  8) & 0xFF)
                            << (char)((binary >>  0) & 0xFF);
        } else {
            // compileLine (should have) printed an error.  We don't have to print one here.
            break;
        }
    }
}
