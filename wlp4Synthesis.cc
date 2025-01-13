#include "wlp4Synthesis.h"

void Wlp4Synthesis::codeGenPush(int registerNum, std::ostream& out) {
    out << std::endl;
    out << ";  push($" << registerNum << ")" << std::endl;
    out << "sw $" << registerNum << ", -4($30)" << std::endl;
    out << "sub $30, $30, $4" << std::endl;
}

void Wlp4Synthesis::codeGenPop(int registerNum, std::ostream& out) {
    out << std::endl;
    out << ";  pop($" << registerNum << ")" << std::endl;
    out << "add $30, $30, $4" << std::endl;
    out << "lw $" << registerNum << ", -4($30)" << std::endl;
}

bool Wlp4Synthesis::computeStatement(TreeNode* node, std::string currentProcedureName, std::ostream& out) {
    if (node->getKind() == "statements") { 
        if (node->getChildCount() == 2) { // checks if this is statement .EMPTY
            if (!computeStatement(node->getChild(0), currentProcedureName, out)) return false;
            if (!computeStatement(node->getChild(1), currentProcedureName, out)) return false;
        }
    } else if (node->getKind() == "statement") {
        if (node->getChildCount() == 4) { // statement → lvalue BECOMES expr SEMI
            if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
            codeGenPush(3, out);
            if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
            codeGenPop(5, out);
        } else if (node->getChildCount() == 7) { // statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE
            out << ";  begin while loop" + std::to_string(whileLoopCounter) + "" << std::endl;
            whileLoopCounter++;
            int currentCounter = whileLoopCounter;
            out << "loop" + std::to_string(currentCounter) + ": ";
            if (!computeStatement(node->getChild(2), currentProcedureName, out)) return false;
            out << "beq $3, $0, endWhile" + std::to_string(currentCounter) + "" << std::endl;
            if (!computeStatement(node->getChild(5), currentProcedureName, out)) return false;
            out << "beq $0, $0, loop" + std::to_string(currentCounter) + "" << std::endl;
            out << "endWhile" + std::to_string(currentCounter) + ":" << std::endl;
            out << ";  end while loop" << std::endl;
        } else if (node->getChildCount() == 11) { // statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
            out << ";  begin if block" + std::to_string(ifBlockCounter) + "" << std::endl;
            ifBlockCounter++;
            int currentCounter = ifBlockCounter;
            if (!computeStatement(node->getChild(2), currentProcedureName, out)) return false;
            out << "beq $3, $0, else" + std::to_string(currentCounter) + "" << std::endl;
            if (!computeStatement(node->getChild(5), currentProcedureName, out)) return false;
            out << "beq $0, $0, endif" + std::to_string(currentCounter) + "" << std::endl;
            out << "else" + std::to_string(currentCounter) + ": ";
            if (!computeStatement(node->getChild(9), currentProcedureName, out)) return false;
            out << "endif" + std::to_string(currentCounter) + ":" << std::endl;
            out << ";  end if block" << std::endl;
        } else if (node->getChildCount() == 5) {
            if (node->getChild(2)->getKind() == "expr" && node->getChild(0)->getKind() == "PUTCHAR") {
                if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
                out << ";  putchar() - printing" << std::endl;
                codeGenPush(5, out);
                out << "lis $5" << std::endl;
                out << ".word 0xFFFF000c" << std::endl;
                out << "sw $3, 0($5)" << std::endl;
                codeGenPop(5, out);
            } else if (node->getChild(2)->getKind() == "expr" && node->getChild(0)->getKind() == "PRINTLN") {
                if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
                out << ";  println() - printing" << std::endl;
                codeGenPush(1, out);
                out << "add $1, $3, $0" << std::endl;
                out << "jalr $10" << std::endl;
                codeGenPop(1, out);
            } else if (node->getChild(3)->getKind() == "expr") { // DELETE
                if (!computeExpressions(node->getChild(3), currentProcedureName, out)) return false;
                out << ";  begin delete block" + std::to_string(deleteCounter) + "" << std::endl;
                deleteCounter++;
                int currentCounter = deleteCounter;
                out << "beq $3, $11, skipDelete" + std::to_string(currentCounter) + "" << std::endl;
                codeGenPush(1, out);
                out << "add $1, $3, $0" << std::endl;
                out << "lis $3" << std::endl;
                out << ".word delete" << std::endl;
                out << "jalr $3" << std::endl;
                codeGenPop(1, out);
                out << "skipDelete" + std::to_string(currentCounter) + ":" << std::endl;
            }
        }
    } else if (node->getKind() == "test") {
        if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
        codeGenPush(3, out);
        if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
        codeGenPop(5, out);

        out << ";  comparison" << std::endl;
        if (node->getChild(0)->getType() == INT) {
            if (node->getChild(1)->getKind() == "EQ") {
                out << "sub $3, $5, $3" << std::endl;
                out << "slt $5, $3, $0" << std::endl;
                out << "slt $3, $0, $3" << std::endl;
                out << "add $3, $5, $3" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "NE") {
                out << "sub $3, $5, $3" << std::endl;
                out << "slt $5, $3, $0" << std::endl;
                out << "slt $3, $0, $3" << std::endl;
                out << "add $3, $5, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "LT") {
                out << "slt $3, $5, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "LE") {
                out << "slt $3, $3, $5" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "GT") {
                out << "slt $3, $3, $5" << std::endl;
            } else if (node->getChild(1)->getKind() == "GE") {
                out << "slt $3, $5, $3" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            }
        } else {
            if (node->getChild(1)->getKind() == "EQ") {
                out << "sub $3, $5, $3" << std::endl;
                out << "sltu $5, $3, $0" << std::endl;
                out << "sltu $3, $0, $3" << std::endl;
                out << "add $3, $5, $3" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "NE") {
                out << "sub $3, $5, $3" << std::endl;
                out << "sltu $5, $3, $0" << std::endl;
                out << "sltu $3, $0, $3" << std::endl;
                out << "add $3, $5, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "LT") {
                out << "sltu $3, $5, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "LE") {
                out << "sltu $3, $3, $5" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "GT") {
                out << "sltu $3, $3, $5" << std::endl;
            } else if (node->getChild(1)->getKind() == "GE") {
                out << "sltu $3, $5, $3" << std::endl;
                out << "sub $3, $11, $3" << std::endl;
            }
        }
    }
    return true;
}

bool Wlp4Synthesis::computeExpressions(TreeNode* node, std::string currentProcedureName, std::ostream& out) {
    // factor NULL
    // factor AMP lvalue
    // factor STAR factor
    // factor NEW INT LBRACK expr RBRACK
    // factor ID LPAREN RPAREN
    // factor ID LPAREN arglist RPAREN
    // factor GETCHAR LPAREN RPAREN
    if (node->getKind() == "expr") {
        if (node->getChildCount() == 1) { // expr term
            if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
        } else if (node->getChildCount() == 3) {
            if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
            codeGenPush(3, out);
            if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
            codeGenPop(5, out);
            if (node->getChild(1)->getKind() == "PLUS") {
                if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT) {
                    out << "mult $3, $4" << std::endl;
                    out << "mflo $3" << std::endl;
                } else if (node->getChild(0)->getType() == INT && node->getChild(2)->getType() == INT_STAR) {
                    out << "mult $5, $4" << std::endl;
                    out << "mflo $5" << std::endl;
                }
                out << "add $3, $5, $3" << std::endl; // expr expr PLUS term
            } else if (node->getChild(1)->getKind() == "MINUS") {
                if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT) {
                    out << "mult $3, $4" << std::endl;
                    out << "mflo $3" << std::endl;
                }
                out << "sub $3, $5, $3" << std::endl; // expr expr MINUS term

                if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT_STAR) {
                    out << "div $3, $4" << std::endl;
                    out << "mflo $3" << std::endl;
                }
            } 
        } 
    } else if (node->getKind() == "term") {
        if (node->getChildCount() == 1) { // term factor
            if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
        } else if (node->getChildCount() == 3) {
            if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
            codeGenPush(3, out);
            if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
            codeGenPop(5, out);
            if (node->getChild(1)->getKind() == "STAR") { // term term STAR factor
                out << "mult $3, $5" << std::endl;
                out << "mflo $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "SLASH") { // term term SLASH factor
                out << "div $5, $3" << std::endl;   ///!!!! add a divide by 0 check here by chhanging function header to return bool
                out << "mflo $3" << std::endl;
            } else if (node->getChild(1)->getKind() == "PCT") { // term term PCT factor
                out << "div $5, $3" << std::endl;
                out << "mfhi $3" << std::endl;
            }
        } 
    } else if (node->getKind() == "factor") {
        if (node->getChildCount() == 1) { 
            if (node->getChild(0)->getKind() == "NUM") { // factor NUM
                out << "lis $3" << std::endl;
                out << ".word " + node->getChild(0)->getLexeme() + "" << std::endl;
            } else if (node->getChild(0)->getKind() == "ID") { // factor ID
                std::string location = std::to_string(semanticAnalysis.getTable(currentProcedureName).memoryLocation.at(node->getChild(0)->getLexeme()));
                out << "lw $3, " + location + "($29)" << std::endl;
            } else if (node->getChild(0)->getKind() == "NULL") {
                out << "add $3, $11, $0" << std::endl;
            }
        } else if (node->getChildCount() == 2) {
            if (node->getChild(0)->getKind() == "AMP") {
                if (!computeExpressions(node->getChild(1), currentProcedureName, out)) return false;
            } else if (node->getChild(0)->getKind() == "STAR") {
                if (!computeExpressions(node->getChild(1), currentProcedureName, out)) return false;
                out << "lw $3, 0($3)" << std::endl;
            }
        } else if (node->getChildCount() == 3) { // factor LPAREN expr RPAREN
            if (node->getChild(1)->getKind() == "expr") {
                if (!computeExpressions(node->getChild(1), currentProcedureName, out)) return false;
            } else if (node->getChild(0)->getKind() == "GETCHAR") {
                out << ";  getchar() - reading" << std::endl;
                codeGenPush(5, out);
                out << "lis $5" << std::endl;
                out << ".word 0xFFFF0004" << std::endl;
                out << "lw $3, 0($5)" << std::endl;
                codeGenPop(5, out);
            } else if (node->getChild(0)->getKind() == "ID") {
                codeGenPush(29, out);
                codeGenPush(31, out);

                out << "lis $5" << std::endl; // procedure call
                out << ".word " + PREFIX_NAME + "" + node->getChild(0)->getLexeme() + "" << std::endl;
                out << "jalr $5" << std::endl;  

                codeGenPop(31, out);
                codeGenPop(29, out);
            }
        } else if (node->getChildCount() == 4) {
            if (node->getChild(2)->getKind() == "arglist") {
                int paramCount = semanticAnalysis.getTable(node->getChild(0)->getLexeme()).signature.size();
                codeGenPush(29, out);
                codeGenPush(31, out);

                if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;

                out << "lis $5" << std::endl; // procedure call
                out << ".word "  + PREFIX_NAME + "" + node->getChild(0)->getLexeme() + "" << std::endl;
                out << "jalr $5" << std::endl;  
                
                for (int i = 0; i < paramCount; ++i) { // pop n times
                    out << "add $30, $30, $4" << std::endl;
                }
                codeGenPop(31, out);
                codeGenPop(29, out);
            }
        } else if (node->getChildCount() == 5) { // factor → NEW INT LBRACK expr RBRACK
            if (node->getChild(3)->getKind() == "expr") {
                if (!computeExpressions(node->getChild(3), currentProcedureName, out)) return false;
                out << ";  running new()" << std::endl;
                codeGenPush(1, out);
                out << "add $1, $3, $0" << std::endl;
                out << "lis $3" << std::endl;
                out << ".word new" << std::endl;
                out << "jalr $3" << std::endl;
                out << "bne $3, $0, 1" << std::endl;
                out << "add $3, $11, $0" << std::endl;
                codeGenPop(1, out);
            }
        }
    } else if (node->getKind() == "lvalue") {
        if (node->getChildCount() == 1) {
            std::string location = std::to_string(semanticAnalysis.getTable(currentProcedureName).memoryLocation.at(node->getChild(0)->getLexeme()));
            out << "lis $3" << std::endl;
            out << ".word " + location + "" << std::endl;
            out << "add $3, $3, $29" << std::endl;
        } else if (node->getChildCount() == 2) { // lvalue → STAR factor
            if (!computeExpressions(node->getChild(1), currentProcedureName, out)) return false; // basically just getting the location in memory of this thing
        } else if (node->getChildCount() == 3) {
            if (!computeExpressions(node->getChild(1), currentProcedureName, out)) return false;
        }
    } else if (node->getKind() == "arglist") {
        if (!computeExpressions(node->getChild(0), currentProcedureName, out)) return false;
        codeGenPush(3, out);
        if (node->getChildCount() != 1) if (!computeExpressions(node->getChild(2), currentProcedureName, out)) return false;
    }
    return true;
}

bool Wlp4Synthesis::storeDeclarations(TreeNode* node, std::string currentProcedureName, std::ostream& out, std::string value, bool isHeader, int& declarationCount) { // finds all declarations recursively
    if (node->getKind() == "dcl") {
        if (isHeader && currentProcedureName == "wain") {
            out << "sw " + value + ", -4($30)" << std::endl;
        } else if (!isHeader) {
            if (value == "NULL") {
                out << "sw $11, -4($30)" << std::endl;
            } else {
                out << "lis $3" << std::endl;
                out << ".word " + value + "" << std::endl;
                out << "sw $3, -4($30)" << std::endl;
            }
        }

        out << "sub $30, $30, $4" << std::endl;
        declarationCount++;
    } else if (node->getKind() == "dcls" && node->getLexeme() != EMPTY) { // dcls .EMPTY
        // recurse since dcls dcls dcl BECOMES NUM SEMI/dcls dcls dcl BECOMES NULL SEMI
        if (!storeDeclarations(node->getChild(0), currentProcedureName, out, node->getChild(3)->getLexeme(), isHeader, declarationCount)) return false;
        if (!storeDeclarations(node->getChild(1), currentProcedureName, out, node->getChild(3)->getLexeme(), isHeader, declarationCount)) return false;
    }
    return true;
}

bool Wlp4Synthesis::parseProcedure(TreeNode* node, std::ostream& out) {
    std::string currentProcedureName = node->getChild(1)->getLexeme();
    int declarationCount = 0;
    if (currentProcedureName == "wain") { // main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
        out << "\n;  wain procedure()" << std::endl;
        out << PREFIX_NAME + "wain: ";
        if (!storeDeclarations(node->getChild(3), currentProcedureName, out, "$1", true, declarationCount)) return false; // first dcl at 3
        if (!storeDeclarations(node->getChild(5), currentProcedureName, out, "$2", true, declarationCount)) return false; // second dcl at 5
        out << "sub $29, $30, $4" << std::endl; // setting the frame pointer

        if (!storeDeclarations(node->getChild(8), currentProcedureName, out, "unknown", false, declarationCount)) return false;
        if (!computeStatement(node->getChild(9), currentProcedureName, out)) return false;
        if (!computeExpressions(node->getChild(11), currentProcedureName, out)) return false; // RETURN expr
        out << ";  begin epilogue" << std::endl;
        for (int i = 0; i < declarationCount; ++i) { // clean up the stack
            out << "add $30, $30, $4" << std::endl;
        }
    } else { // procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
        out << "\n;  " + currentProcedureName +" procedure()" << std::endl;
        out << PREFIX_NAME + "" + currentProcedureName + ": ";
        out << "sub $29, $30, $4" << std::endl; // setting the frame pointer
        if (!storeDeclarations(node->getChild(6), currentProcedureName, out, "unknown", false, declarationCount)) return false;
        codeGenPush(5, out);
        codeGenPush(6, out);
        codeGenPush(7, out);
        if (!computeStatement(node->getChild(7), currentProcedureName, out)) return false;
        if (!computeExpressions(node->getChild(9), currentProcedureName, out)) return false; // RETURN expr
        codeGenPop(7, out);
        codeGenPop(6, out);
        codeGenPop(5, out);
        out << ";  begin epilogue" << std::endl;
        for (int i = 0; i < declarationCount; ++i) { // clean up the stack
            out << "add $30, $30, $4" << std::endl;
        }
        out << "jr $31" << std::endl;
    }
    return true;
}

void Wlp4Synthesis::preamble(std::ostream& out) {
    out << ";  begin prologue" << std::endl;
    out << ".import print" << std::endl;
    out << ".import init" << std::endl;
    out << ".import new" << std::endl;
    out << ".import delete" << std::endl;
    out << "lis $4" << std::endl;
    out << ".word 4" << std::endl;
    out << "lis $10" << std::endl;
    out << ".word print" << std::endl; // optimization if no print is needed
    out << "lis $11" << std::endl;
    out << ".word 1" << std::endl;
    out << "sw $31, -4($30)" << std::endl;
    out << "sub $30, $30, $4" << std::endl;
    if (semanticAnalysis.getExpectsArray()) {
        out << "lis $3" << std::endl; // calling init at the start of main
        out << ".word init" << std::endl;
        out << "jalr $3" << std::endl;
    } else {
        codeGenPush(2, out);
        out << "add $2, $0, $0" << std::endl;
        out << "lis $3" << std::endl; // calling init at the start of main
        out << ".word init" << std::endl;
        out << "jalr $3" << std::endl;
        codeGenPop(2, out);
    }
    out << std::endl;
    out << ";  end prologue" << std::endl;
    out << "beq $0, $0, " + PREFIX_NAME + "wain" << std::endl;
    beginParsing(out);
    out << "add $30, $30, $4" << std::endl;
    out << "lw $31, -4($30)" << std::endl;
    out << "jr $31" << std::endl;
    system("./helper.sh");
}

bool Wlp4Synthesis::treeTraversal(TreeNode* node, std::ostream& out) { // passed in procedures in the out
    if (node->getChildCount() == 1) {
        if (!parseProcedure(node->getChild(0), out)) return false;
    } else {
        if (!parseProcedure(node->getChild(0), out)) return false;
        if (!treeTraversal(node->getChild(1), out)) return false;
    }
    return true;
}

void Wlp4Synthesis::beginParsing(std::ostream& out) {
    if (!treeTraversal(semanticAnalysis.getRoot()->getChild(1), out)) std::cerr << "ERROR" << std::endl;
    //else treeUtil.treePrinter(root);
}

Wlp4Synthesis::Wlp4Synthesis()
    : whileLoopCounter{0}, ifBlockCounter{0}, deleteCounter{0} {}

void Wlp4Synthesis::generate() {
    semanticAnalysis.analyse();
    std::ofstream mipsSourceCode;
    mipsSourceCode.open("!4.mipsSourceCode.asm", std::fstream::out);
    preamble(mipsSourceCode); // begin code generation
}
