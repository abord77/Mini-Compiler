#include "treeNode.h"

TreeNode::TreeNode(std::string kind, std::string lexeme)
    : kind{kind}, lexeme{lexeme}, type{NO_TYPE}, children{} {}

void TreeNode::setType(DataType type) {
    this->type = type;
}

void TreeNode::setLexeme(std::string lexeme) {
    this->lexeme = lexeme;
}

std::string TreeNode::getKind() {
    return kind;
}

std::string TreeNode::getLexeme() {
    return lexeme;
}

DataType TreeNode::getType() {
    return type;
}

TreeNode* TreeNode::getChild(int index) {
    return children.at(index).get();
}

void TreeNode::addChild(std::unique_ptr<TreeNode> node) {
    children.emplace_back(std::move(node));
}

int TreeNode::getChildCount() {
    return children.size();
}

std::ostream & operator<<(std::ostream & out, TreeNode* node) {
    out << node->getKind() << " " << node->getLexeme();
    if(node->getType() != NO_TYPE) {
        std::string dataType = std::regex_replace(dataTypeToString(node->getType()), std::regex("_STAR"), "*");
        std::transform(dataType.begin(), dataType.end(), dataType.begin(), ::tolower);
        out << " : " << dataType;
    }
    return out;
}
