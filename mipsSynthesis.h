#ifndef __MIPSSYNTHESIS_H__
#define __MIPSSYNTHESIS_H__
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

class MipsSynthesis {
    /** Matches a line of mips assembly */
    const std::regex ASM_LINE_PATTERN;

    /** Recognizes an empty line (or an empty line with a comment) */
    const std::regex EMPTY_LINE;

    /** Prints an error to stderr with an "ERROR: " prefix, and newline suffix.
     *
     * @param message The error to print
     */
    void formatError(const std::string & message);

    /** Checks if the value of the parameters of a register-format instruction are valid
     *
     * @param one The value of the first parameter
     * @param two The value of the second parameter
     * @param three The value of the third parameter
     * @param instruction The name of the instruction
     * @return Whether the value of the parameters are of valid registers
     */
    bool registerFormatInputCheck(uint32_t one, uint32_t two, uint32_t three, std::string instruction);

    /** Checks if the value of the parameters of a immediate-format instruction are valid
     *
     * @param one The value of the first parameter
     * @param two The value of the second parameter
     * @param three The value of the third parameter
     * @param isBranch True if this immediateFormat is a branch instruction or not
     * @param instruction The name of the instruction
     * @return Whether the value of the parameters are of valid registers and immediate does not overflow
     */
    bool immediateFormatInputCheck(uint32_t one, uint32_t two, uint32_t three, bool isBranch, std::string instruction);

    /** Calculates the binary value of a decimal number for register values
     *
     * @param decimalVal Is a decimal number <= 31
     * @return Binary representation of the register value
     */
    std::string decimalToBinaryRegister(uint32_t decimalVal);

    /** Calculates the binary value of a decimal number for immediate values
     *
     * @param decimalVal Is a decimal number <= 65535
     * @return Binary representation of the immediate value
     */
    std::string decimalToBinaryImmediate(uint32_t decimalVal);

    /** Calculates the binary value of a decimal number for words
     *
     * @param decimalVal Is a decimal number <= 4294967295
     * @return Binary representation of the word value
     */
    std::string decimalToBinaryWord(uint32_t decimalVal);

    /** Calculates the decimal value given a binary string
     *
     * @param binaryVal Is a string of 0s and 1s
     * @return Decimal representation of the binary number
     */
    int binaryToDecimal(std::string binaryVal);

    /** For a given instruction, returns the machine code for that instruction.
     *
     * @param[out] word The machine code for the instruction
     * @param instruction The name of the instruction
     * @param one The value of the first parameter
     * @param two The value of the second parameter, 0 if the instruction has < 2 parameter
     * @param three The value of the third parameter, 0 if the instruction has < 3 parameter
     */
    bool compileLine(uint32_t &          word,
                    const std::string & instruction,
                    uint32_t            one,
                    uint32_t            two,
                    uint32_t            three);
    
    /** Maps the instruction name to the parameter type.  The value must be a 3 character string, 'r'
     *  represents a register, and 'i' represents an immediate. */


    /** Convert a string representation of a number to an unsigned integer.
     *
     * If the string is "0", then 0 is returned.  If the string starts with "0x", the string is
     * interpreted as an unsigned hexidecimal number.  If the string starts with a "0", the string is
     * interpreted as an unsigned octal number.  Otherwise, the string is interpreted as a signed
     * decimal number.
     *
     * The function name is read as "string to uint32".
     *
     * @param s The string to parse
     * @return The uint32_t representation of the string
     */
    uint32_t stouint32(const std::string & s);

    /** Compiles one line of assembly and send the binary to standard out.  If the assembly is invalid,
     *  print an error to stderr and return false.  Assumes that the assembly does not have a trailing
     *  comment.
     *
     * @param line The line to parse
     * @return True if the line is valid assembly and was output to stdout, false otherwise
     */
    bool parseLine(const std::string & line, const std::ofstream &out);

    public:
        MipsSynthesis();

        /** Entrypoint for the assembler.  The first parameter (optional) is a mips assembly file to
         *  read.  If no parameter is specified, read assembly from stdin.  Prints machine code to stdout.
         *  If invalid assembly is found, prints an error to stderr, stops reading assembly, and return a
         *  non-0 value.
         *
         * If the file is not found, print an error and returns a non-0 value.
         *
         * @return 0 on success, non-0 on error
         */
        void generate();
};

#endif
