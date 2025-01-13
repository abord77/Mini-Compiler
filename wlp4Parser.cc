#include "wlp4Parser.h"

void Wlp4Parser::readTokens() {
    std::string s;
    std::ifstream tokenization;
    tokenization.open("!1.wlp4Tokens.txt", std::fstream::in);
    while(std::getline(tokenization, s)) {
        std::istringstream inputWords{s};
        std::string inputCharKind;
        std::string inputCharLexeme;
        inputWords >> inputCharKind >> inputCharLexeme;
        parsingProgress.emplace_back(std::unique_ptr<TreeNode>(new TreeNode{inputCharKind, inputCharLexeme}));
        parsingOutput.emplace_back(inputCharKind);
    }
    parsingProgress.emplace_back(std::unique_ptr<TreeNode>(new TreeNode{"EOF", "EOF"}));
    parsingOutput.emplace_back("EOF");
}

void Wlp4Parser::printParseTree(TreeNode* node, std::ofstream& out) {
    if (node->getChildCount() != 0) {
        out << node << std::endl;
        for (int i = 0; i < node->getChildCount(); ++i) {
            printParseTree(node->getChild(i), out);
        }
    } else {
        out << node << std::endl;
    }
}

void Wlp4Parser::tryReduce(int i) {
    while (true) {
        int reductionRule = cfg.getReduction(stateStack.top(), parsingProgress[i]->getKind());
        if (reductionRule == -1) break;
        std::vector<std::unique_ptr<TreeNode>> creatingNewNode;
        std::string productionRHS = "";
        for (int j = 0; j < cfg.getProduction(reductionRule).second.size(); ++j) {
            if (cfg.getProduction(reductionRule).second[0] == ".EMPTY") {
                productionRHS = ".EMPTY";
                break;
            }
            creatingNewNode.emplace_back(std::move(symStack.top()));
            symStack.pop();
            stateStack.pop();
            productionRHS += cfg.getProduction(reductionRule).second[j] + (j == cfg.getProduction(reductionRule).second.size() - 1 ? "" : " ");
        }
        std::string productionLHS = cfg.getProduction(reductionRule).first;
        symStack.push(std::unique_ptr<TreeNode>(new TreeNode{productionLHS, productionRHS}));
        for (int j = creatingNewNode.size() - 1; j >= 0; --j) {
            symStack.top()->addChild(std::move(creatingNewNode[j]));
        }
        int toState = cfg.getTransition(stateStack.top(), productionLHS);
        if (toState == -1) break;
        stateStack.push(toState);
    }
}

Wlp4Parser::Wlp4Parser(): completedSuccessfully{true} {
    parsingOutput.emplace_back(".");
    parsingOutput.emplace_back("BOF");
    parsingProgress.emplace_back(std::unique_ptr<TreeNode>(new TreeNode{"BOF", "BOF"}));
    stateStack.push(0);
    cfg.init();
}

void Wlp4Parser::parse() {
    readTokens();
    int currentReadingPosition = 0, currentReadingPositionOutput = 0;
    for (int i = 0; i < parsingProgress.size(); ++i) {
        tryReduce(i);
        symStack.push(std::unique_ptr<TreeNode>(new TreeNode{parsingProgress[i]->getKind(), parsingProgress[i]->getLexeme()}));
        int transitionRule = cfg.getTransition(stateStack.top(), parsingProgress[i]->getKind());
        if (transitionRule != -1) {
            stateStack.push(transitionRule);
        } else {
            completedSuccessfully = false;
            std::cerr << "ERROR at " << currentReadingPosition << std::endl;
            break;
        }
        currentReadingPosition++;
    }
    
    int symStackSize = symStack.size();
    if (completedSuccessfully) {
        std::string productionRHS = "";
        root = std::make_unique<TreeNode>(cfg.getProduction(0).first, productionRHS);
        std::vector<std::unique_ptr<TreeNode>> rootNodeChildren;
        for (int i = 0; i < symStackSize; ++i) {
            rootNodeChildren.emplace_back(std::move(symStack.top()));
            symStack.pop();
        }
        for (int i = rootNodeChildren.size() - 1; i >= 0; --i) {
            productionRHS += rootNodeChildren[i]->getKind() + (i == 0 ? "" : " ");
            root->addChild(std::move(rootNodeChildren[i]));
        }
        root->setLexeme(productionRHS);

        std::ofstream wlp4ParseTree;
        wlp4ParseTree.open("!2.wlp4ParseTree.txt", std::fstream::out);
        printParseTree(root.get(), wlp4ParseTree);
    }
}

TreeNode* Wlp4Parser::getRoot() {
    return root.get();
}
