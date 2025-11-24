#include "interfaces/SemanticAnalyzer.hpp"

namespace LynxAnalyzer {

    void SemanticAnalyzer::analyzeASTParallel(const Node* /*node*/) { }

    void SemanticAnalyzer::analyzeNodeParallel(Node* /*node*/, SemanticThreadPool& /*pool*/) { }
    
    void SemanticAnalyzer::addSymbolThreadSafe(const std::string& /*name*/, const std::string& /*type*/) { }

    bool SemanticAnalyzer::symbolExists(const std::string& /*name*/) { return false; }

    const std::unordered_map<std::string, std::string>& SemanticAnalyzer::getSymbolTable() const { return symbolTable; }

    void SemanticAnalyzer::reportError(const std::string& /*msg*/, int /*lineNo*/) { }

    bool SemanticAnalyzer::hasErrors() const { return false; }

}
