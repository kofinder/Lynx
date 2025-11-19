#include "state/ClassState.hpp"
#include <ast/FunctionNode.hpp>
#include <interfaces/SemanticAnalyzer.hpp>
#include <ast/ClazzDeclarationNode.hpp>
#include <ast/VariableDeclarationNode.hpp>

namespace LynxAnalyzer {

    using namespace LynxAst;

    void ClassState::handleNode(SemanticAnalyzer& analyzer, Node* node) {
        if (!node) return;
        if (auto* clazz = dynamic_cast<ClazzDeclarationNode*>(node)) {
            std::cout << "[Semantic] Found class: " << clazz->getQualifiedClassName() << "\n";
            analyzer.addSymbolThreadSafe(clazz->getQualifiedClassName(), "class");
        }

        else if (auto* func = dynamic_cast<FunctionNode*>(node)) {
            std::cout << "[Semantic] Found method: " << func->getFunctionName() << "\n";
            analyzer.addSymbolThreadSafe(func->getFunctionName(), "method");
        }

        else if (auto* var = dynamic_cast<VariableDeclarationNode*>(node)) {
            std::cout << "[Semantic] Found field: " << var->getVariableName() << "\n";
            analyzer.addSymbolThreadSafe(var->getVariableName(), "field");
        }
        
    }

    bool ClassState::canAccessField(const std::string& currentClass, const FieldType* method) const { return false; }

    bool ClassState::canAccessMethod(const std::string& currentClass, const MethodType* method) const { return false; }

    bool ClassState::isSubclass(const std::string& derived, const std::string& base) const { return false; }

}