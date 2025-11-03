#include "IdentifierNode.hpp"
#include <logger/Logger.hpp>
#include "utils/ClassUtils.hpp"
#include "utils/VariableUtils.hpp"
#include <types/tmpl/TypeChecker.hpp>


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    
    llvm::Value* IdentifierNode::generateCode(std::shared_ptr<AstContext> astContext) {

        auto symbol = astContext->getGlobalContext();

        if (auto* derefVariable = symbol->findLocalVariable(identifierName)) {
            LOG_INFO("[Resolve] Identifier '{}' found in global/local symbol table", identifierName);
            return derefVariable;
        }  

        if(auto* derefVariable = ClassUtils::resolveSuperMethod(*astContext, identifierName)) {
            LOG_INFO("No super instance found for current class");
            return derefVariable;
        } 

        if(auto* derefVariable = ClassUtils::getFieldFromThis(*astContext, identifierName)) {
            LOG_INFO("[Resolve] Identifier '{}' found as class member", identifierName);
            return derefVariable;
        }

        if(auto* derefVariable = VariableUtils::resolveVariable(astContext.get(), identifierName).value) {
            LOG_INFO("[Resolve] Identifier '{}' found as local variable", identifierName);
            return derefVariable;
        }

        LOG_ERROR("[IdentifierNode] Could not resolve identifier: {}", identifierName);        
        return nullptr;
    }

    std::unique_ptr<Node> IdentifierNode::clone() const {
        return std::make_unique<IdentifierNode>(*this); 
    }
        
}

