#include "wlp4SemanticAnalysis.h"

void SemanticAnalyzer::printParseTree(TreeNode* node, std::ofstream &out) {
    if (node->getChildCount() != 0) {
        out << node << std::endl;
        for (int i = 0; i < node->getChildCount(); ++i) {
            printParseTree(node->getChild(i), out);
        }
    } else {
        out << node << std::endl;
    }
}

DataType SemanticAnalyzer::typeInference(TreeNode* node, std::string currentProcedureName, std::string procedureName) { // deduces what type anything can be
    if (node->getKind() == "expr") {
        if (node->getChildCount() == 1) return node->getChild(0)->getType();             // expr → term
        else if (node->getChildCount() == 3 && node->getChild(1)->getKind() == "PLUS") { // expr → expr PLUS term (3 cases) 
            if (node->getChild(0)->getType() == INT && node->getChild(2)->getType() == INT) return INT;
            else if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT) return INT_STAR;
            else if (node->getChild(0)->getType() == INT && node->getChild(2)->getType() == INT_STAR) return INT_STAR;
            else return ERROR;
        } 
        else if (node->getChildCount() == 3 && node->getChild(1)->getKind() == "MINUS") { // expr → expr MINUS term (3 cases)
            if (node->getChild(0)->getType() == INT && node->getChild(2)->getType() == INT) return INT; 
            else if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT) return INT_STAR;
            else if (node->getChild(0)->getType() == INT_STAR && node->getChild(2)->getType() == INT_STAR) return INT;
            else return ERROR;
        }
        else return ERROR;
    } else if (node->getKind() == "term") {
        if (node->getChildCount() == 1) return node->getChild(0)->getType();                                                         // term → factor
        else if (node->getChildCount() == 3 && node->getChild(0)->getType() == INT && node->getChild(2)->getType() == INT) return INT;    // term → term STAR factor
        else return ERROR;
    } else if (node->getKind() == "factor") {
        if (node->getChildCount() == 1) return node->getChild(0)->getType();                                                                // factor → ID | NUM | NULL
        else if (node->getChildCount() == 2 && node->getChild(1)->getKind() == "lvalue" && node->getChild(1)->getType() == INT) return INT_STAR; // factor → AMP lvalue
        else if (node->getChildCount() == 2 && node->getChild(1)->getKind() == "factor" && node->getChild(1)->getType() == INT_STAR) return INT; // factor → STAR factor
        else if (node->getChildCount() == 3 && node->getChild(1)->getKind() == "expr") return node->getChild(1)->getType();                      // factor → LPAREN expr RPAREN
        else if (node->getChildCount() == 5 && node->getChild(3)->getKind() == "expr" && node->getChild(3)->getType() == INT) return INT_STAR;   // factor → NEW INT LBRACK expr RBRACK
        else if (node->getChildCount() == 3 && node->getChild(0)->getKind() == "ID") {
            if (tables.at(node->getChild(0)->getLexeme()).signature.size() != 0 || node->getChild(0)->getLexeme() == "wain") return ERROR;
            return INT;
        }
        else if (node->getChildCount() == 4 && node->getChild(0)->getKind() == "ID" && node->getChild(0)->getLexeme() != "wain") return INT;
        else if (node->getChildCount() == 3 && node->getChild(0)->getKind() == "GETCHAR") return INT;
        else return ERROR;
    } else if (node->getKind() == "lvalue") {
        if (node->getChildCount() == 1) return node->getChild(0)->getType(); //if (node->getChild(0)->getKind() == "ID" && tables[currentProcedureName).variables.count(node->getChild(0)->getLexeme()) == 1) return tables[currentProcedureName).variables.at(node->getChild(0)->getLexeme());
        else if (node->getChildCount() == 2 && node->getChild(1)->getKind() == "factor" && node->getChild(1)->getType() == INT_STAR) return INT; // lvalue → STAR factor
        else if (node->getChildCount() == 3 && node->getChild(1)->getKind() == "lvalue") return node->getChild(1)->getType();                    // lvalue → LPAREN lvalue RPAREN
        else return ERROR;
    } else if (node->getKind() == "arglist") {
        bool tmp = false;
        if(annotateArglist(node, currentProcedureName, 0, procedureName, tmp)) return NO_TYPE;
        else return ERROR;
    } 

    // base cases
    if (node->getKind() == "NUM") return INT;
    else if (node->getKind() == "NULL") return INT_STAR;
    else if (node->getKind() == "ID" && tables[currentProcedureName].variables.count(node->getLexeme()) == 1) {
        return tables[currentProcedureName].variables.at(node->getLexeme());
    }
    else if (node->getKind() == "ID" && tables.count(node->getLexeme()) == 1 && node->getLexeme() != "wain") return INT;
    else if (node->getKind() == "ID" && tables[currentProcedureName].variables.count(node->getLexeme()) == 0) return ERROR; // tried to do type inference with an undeclared variable
    else if (node->getKind() == "ID" && (tables.count(node->getLexeme()) == 0 || node->getLexeme() == "wain")) return ERROR; // tried to do type inference with an undeclared procedure
    
    return node->getType();
}

bool SemanticAnalyzer::annotateArglist(TreeNode* node, std::string currentProcedureName, int currentParam, std::string procedureName, bool& enoughArguments) { // checks if each param type matches with the required arglist recursively
    if (node->getChildCount() == 3) {
        if (!annotateArglist(node->getChild(2), currentProcedureName, currentParam + 1, procedureName, enoughArguments)) return false;
    }
    DataType expressionType = annotateTypes(node->getChild(0), currentProcedureName);
    if (expressionType == ERROR) return false;
    if (tables.at(procedureName).signature.size() <= currentParam || tables.at(procedureName).signature[currentParam] != expressionType) return false;
    if (enoughArguments || tables.at(procedureName).signature.size() == currentParam + 1) {
        enoughArguments = true; 
        return true;
    }
    return false;
}

DataType SemanticAnalyzer::annotateTypes(TreeNode* node, std::string currentProcedureName) { // typeChecking for expressions
    for (int i = node->getChildCount() - 1; i >= 0; --i) { // annotate all children
        DataType childType;
        if (node->getChild(i)->getKind() == "arglist") {
            if (typeInference(node->getChild(i), currentProcedureName, node->getChild(0)->getLexeme()) == ERROR) return ERROR;
        }
        else childType = annotateTypes(node->getChild(i), currentProcedureName);
        if (childType == ERROR) return ERROR;
        if ((node->getLexeme() == "ID LPAREN arglist RPAREN" || node->getLexeme() == "ID LPAREN RPAREN") && node->getChild(i)->getKind() == "ID") node->getChild(i)->setType(NO_TYPE); 
    }

    DataType currentNodeType = typeInference(node, currentProcedureName); // figure out what type node is based off children
    if (currentNodeType == ERROR) return ERROR;
    node->setType(currentNodeType);
    return node->getType();
}

bool SemanticAnalyzer::findDeclarations(TreeNode* node, std::string currentProcedureName, bool isHeader) { // finds all declarations recursively
    if (node->getKind() == "dcl") {
        std::string type = node->getChild(0)->getChild(0)->getKind();
        if (node->getChild(0)->getChildCount() == 2) type += "_" + node->getChild(0)->getChild(1)->getKind();
        if (isHeader) tables[currentProcedureName].signature.emplace_back(stringToDataType(type));
        if (tables[currentProcedureName].variables.count(node->getChild(1)->getLexeme()) == 1) return false;
        tables[currentProcedureName].variables.insert(std::make_pair(node->getChild(1)->getLexeme(), stringToDataType(type)));
        if (isHeader) {
            for (auto &p : tables[currentProcedureName].memoryLocation) {
                p.second += 4;
            }
            tables[currentProcedureName].memoryLocation.insert(std::make_pair(node->getChild(1)->getLexeme(), 4));
        } else {
            for (auto &p : tables[currentProcedureName].memoryLocation) {
                if (p.second < 4) p.second -= 4;
            }
            tables[currentProcedureName].memoryLocation.insert(std::make_pair(node->getChild(1)->getLexeme(), 0));
        }
        node->getChild(1)->setType(stringToDataType(type));
    } else if (node->getKind() == "dcls" && node->getLexeme() != ".EMPTY") { // dcls .EMPTY
        // recurse since dcls dcls dcl BECOMES NUM SEMI/dcls dcls dcl BECOMES NULL SEMI
        if (!findDeclarations(node->getChild(1), currentProcedureName, isHeader)) return false;
        if (!findDeclarations(node->getChild(0), currentProcedureName, isHeader)) return false;

        DataType rhs = typeInference(node->getChild(3), currentProcedureName);
        if (rhs == ERROR) return false;
        if (rhs != node->getChild(1)->getChild(1)->getType()) return false;
        node->getChild(3)->setType(rhs);
    }
    return true;
}

bool SemanticAnalyzer::findParams(TreeNode* node, std::string currentProcedureName) {
    if (node->getChildCount() == 1) {
        if (node->getChild(0)->getKind() == "paramlist") {
            if (!findParams(node->getChild(0), currentProcedureName)) return false;
        } else if (node->getChild(0)->getKind() == "dcl") {
            if (!findDeclarations(node->getChild(0), currentProcedureName, true)) return false;
        }
    } else if (node->getChildCount() == 3) {
        if (!findDeclarations(node->getChild(0), currentProcedureName, true)) return false;
        if (!findParams(node->getChild(2), currentProcedureName)) return false;
    }
    return true;
}

bool SemanticAnalyzer::isWellTyped(TreeNode* node, std::string currentProcedureName) {
    if (node->getKind() == "statements") {
        if (node->getChildCount() == 2) { // checks if this is statement .EMPTY
            if (!isWellTyped(node->getChild(0), currentProcedureName)) return false;
            if (!isWellTyped(node->getChild(1), currentProcedureName)) return false;
        } 
    } else if (node->getKind() == "statement") {
        if (node->getChildCount() == 4) {
            DataType lvalue = annotateTypes(node->getChild(0), currentProcedureName);
            DataType expr = annotateTypes(node->getChild(2), currentProcedureName);
            if (lvalue == ERROR || expr == ERROR) return false;
            if (lvalue != expr) return false;
        } else if (node->getChildCount() == 7) {
            if (!isWellTyped(node->getChild(2), currentProcedureName)) return false;
            if (!isWellTyped(node->getChild(5), currentProcedureName)) return false;
        } else if (node->getChildCount() == 11) {
            if (!isWellTyped(node->getChild(2), currentProcedureName)) return false;
            if (!isWellTyped(node->getChild(5), currentProcedureName)) return false;
            if (!isWellTyped(node->getChild(9), currentProcedureName)) return false;
        } else if (node->getChildCount() == 5) {
            if (node->getChild(2)->getKind() == "expr") {
                DataType expr = annotateTypes(node->getChild(2), currentProcedureName);
                if (expr == ERROR || expr != INT) return false;
            } else if (node->getChild(3)->getKind() == "expr") {
                DataType expr = annotateTypes(node->getChild(3), currentProcedureName);
                if (expr == ERROR || expr != INT_STAR) return false;
            }
        }
    } else if (node->getKind() == "test") {
        DataType lhs = annotateTypes(node->getChild(0), currentProcedureName);
        DataType rhs = annotateTypes(node->getChild(2), currentProcedureName);
        if (lhs == ERROR || rhs == ERROR) return false;
        if (lhs != rhs) return false;
    }
    return true;
}

bool SemanticAnalyzer::parseProcedure(TreeNode* node) {
    std::string currentProcedureName = node->getChild(1)->getLexeme();
    if (tables.count(currentProcedureName) == 1) return false;
    tables.insert(std::make_pair(currentProcedureName, Procedure{}));
    if (currentProcedureName == "wain") { // main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
        if (!findDeclarations(node->getChild(3), currentProcedureName, true)) return false; // first dcl at 3
        if (!findDeclarations(node->getChild(5), currentProcedureName, true)) return false; // second dcl at 5
        if (dataTypeToString(tables[currentProcedureName].signature[0]) == "INT_STAR") expectsArray = true;
        // do a check for the second param being an int
        if (dataTypeToString(tables[currentProcedureName].signature[1]) != "INT") return false;
        if (!findDeclarations(node->getChild(8), currentProcedureName, false)) return false;
        if (!isWellTyped(node->getChild(9), currentProcedureName)) return false;
        if (annotateTypes(node->getChild(11), currentProcedureName) != INT) return false; // RETURN expr
    } else { // procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
        if (!findParams(node->getChild(3), currentProcedureName)) return false;
        if (!findDeclarations(node->getChild(6), currentProcedureName, false)) return false;
        if (!isWellTyped(node->getChild(7), currentProcedureName)) return false;
        if (annotateTypes(node->getChild(9), currentProcedureName) != INT) return false; // RETURN expr
    }
    return true;
}

bool SemanticAnalyzer::treeTraversal(TreeNode* node) { // passed in procedures in the program
    if (node->getChildCount() == 1) {
        if (!parseProcedure(node->getChild(0))) return false;
    } else {
        if (!parseProcedure(node->getChild(0))) return false;
        if (!treeTraversal(node->getChild(1))) return false;
    }
    return true;
}


SemanticAnalyzer::SemanticAnalyzer() {}

TreeNode* SemanticAnalyzer::getRoot() {
    return parser.getRoot();
}

Procedure& SemanticAnalyzer::getTable(std::string currentProcedureName) {
    return tables.at(currentProcedureName);
}

bool SemanticAnalyzer::getExpectsArray() {
    return expectsArray;
}

void SemanticAnalyzer::analyse() {
    std::ofstream wlp4TypedParseTree;
    wlp4TypedParseTree.open("!3.wlp4TypedParseTree.txt", std::fstream::out);

    parser.parse();
    if (!treeTraversal(parser.getRoot()->getChild(1))) std::cerr << "ERROR" << std::endl;
    printParseTree(parser.getRoot(), wlp4TypedParseTree);
}
