#ifndef __TREENODE_H__
#define __TREENODE_H__
#include <string>
#include <vector>
#include <memory>
#include <regex>
#include "dataType.h"

class TreeNode {
    std::string kind;
    std::string lexeme;
    DataType type;
    std::vector<std::unique_ptr<TreeNode>> children;

    public:
        TreeNode(std::string kind, std::string lexeme);
        //void setLexeme(std::string lexeme);
        void setType(DataType type);
        void setLexeme(std::string lexeme);
        std::string getKind();
        std::string getLexeme();
        DataType getType();
        TreeNode* getChild(int index);
        void addChild(std::unique_ptr<TreeNode> node);
        int getChildCount();
        friend std::ostream & operator<<(std::ostream & out, TreeNode* node);
};

#endif
