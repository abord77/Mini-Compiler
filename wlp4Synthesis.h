#ifndef __WLP4SYNTHESIS_H__
#define __WLP4SYNTHESIS_H__
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include "wlp4SemanticAnalysis.h"

class Wlp4Synthesis {
    const std::string EMPTY = ".EMPTY";
    const std::string PREFIX_NAME = "abord77";
    int whileLoopCounter;
    int ifBlockCounter;
    int deleteCounter;
    SemanticAnalyzer semanticAnalysis;

    void codeGenPush(int registerNum, std::ostream& out);
    void codeGenPop(int registerNum, std::ostream& out);

    bool computeStatement(TreeNode* node, std::string currentProcedureName, std::ostream& out);
    bool computeExpressions(TreeNode* node, std::string currentProcedureName, std::ostream& out);
    bool storeDeclarations(TreeNode* node, std::string currentProcedureName, std::ostream& out, std::string value, bool isHeader, int& declarationCount); // finds all declarations recursively
    bool parseProcedure(TreeNode* node, std::ostream& out);
    bool treeTraversal(TreeNode* node, std::ostream& out);
    void beginParsing(std::ostream& out);
    void preamble(std::ostream& out);

    public:
        Wlp4Synthesis();
        void generate();
};

#endif
