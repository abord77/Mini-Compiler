#ifndef __WLP4PARSER_H__
#define __WLP4PARSER_H__
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <stack>
#include <memory>
#include "wlp4CFG.h"
#include "treeNode.h"

class Wlp4Parser {
    std::vector<std::unique_ptr<TreeNode>> parsingProgress;
    std::vector<std::string> parsingOutput;
    std::stack<int> stateStack;
    std::stack<std::unique_ptr<TreeNode>> symStack;
    bool completedSuccessfully;
    Wlp4CFG cfg; // context free grammar for this language
    std::unique_ptr<TreeNode> root; // root of the constructed parse tree

    void readTokens();
    void printParseTree(TreeNode* node, std::ofstream &out);
    void tryReduce(int i);

    public:
        Wlp4Parser();
        void parse();
        TreeNode* getRoot();
};

#endif
