#ifndef __WLP4SEMANTICANALYSIS_H__
#define __WLP4SEMANTICANALYSIS_H__
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include "procedure.h"
#include "treeNode.h"
#include "wlp4Parser.h"

class SemanticAnalyzer {
    bool expectsArray;
    std::map<std::string, Procedure> tables; // global symbol table
    Wlp4Parser parser;

    void printParseTree(TreeNode* node, std::ofstream &out);
    DataType typeInference(TreeNode* node, std::string currentProcedureName, std::string procedureName = "");
    bool annotateArglist(TreeNode* node, std::string currentProcedureName, int currentParam, std::string procedureName, bool& enoughArguments);
    DataType annotateTypes(TreeNode* node, std::string currentProcedureName);
    bool findDeclarations(TreeNode* node, std::string currentProcedureName, bool isHeader);
    bool findParams(TreeNode* node, std::string currentProcedureName);
    bool isWellTyped(TreeNode* node, std::string currentProcedureName);
    bool parseProcedure(TreeNode* node);
    bool treeTraversal(TreeNode* node);

    public:
        SemanticAnalyzer();
        TreeNode* getRoot();
        Procedure& getTable(std::string currentProcedureName);
        bool getExpectsArray();
        void analyse();
};

#endif
