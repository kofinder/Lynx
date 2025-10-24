#include "state/ClassState.hpp"
#include <logger/Logger.hpp>
#include <ast/FunctionNode.hpp>
#include <interfaces/SemanticAnalyzer.hpp>
#include <ast/ClazzDeclarationNode.hpp>
#include <ast/VariableDeclarationNode.hpp>


namespace LynxAnalyzer {

    using namespace LynxAst;
    using namespace LynxLogger;

    void ClassState::handleNode(SemanticAnalyzer& analyzer, Node* node) {
        LOG_ERROR("Invoked");
        if (!node) return;

        // Detect class declaration
        if (auto* clazz = dynamic_cast<ClazzDeclarationNode*>(node)) {
            std::cout << "[Semantic] Found class: " << clazz->getQualifiedClassName() << "\n";
            analyzer.addSymbolThreadSafe(clazz->getQualifiedClassName(), "class");
        }

        // Detect function inside class
        else if (auto* func = dynamic_cast<FunctionNode*>(node)) {
            std::cout << "[Semantic] Found method: " << func->getFunctionName() << "\n";
            analyzer.addSymbolThreadSafe(func->getFunctionName(), "method");
        }

        // Detect field inside class
        else if (auto* var = dynamic_cast<VariableDeclarationNode*>(node)) {
            std::cout << "[Semantic] Found field: " << var->getVariableName() << "\n";
            analyzer.addSymbolThreadSafe(var->getVariableName(), "field");
        }
        
    }

    bool ClassState::canAccessField(const std::string& currentClass, const FieldType* method) const {
        LOG_ERROR("Invoked");
        return false;
    }

    bool ClassState::canAccessMethod(const std::string& currentClass, const MethodType* method) const {
        LOG_ERROR("Invoked");
        return false;
    }

    bool ClassState::isSubclass(const std::string& derived, const std::string& base) const {
        LOG_ERROR("Invoked");
        return false;
    }

}