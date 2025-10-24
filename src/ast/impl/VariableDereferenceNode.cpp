
#include <logger/Logger.hpp>
#include "VariableDereferenceNode.hpp"
#include "VariableDeclarationNode.hpp"
#include <exceptions/BaseException.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>

using namespace LynxExceptions;
using namespace LynxLogger;
using namespace LynxContext;

namespace LynxAst {

    llvm::Value* VariableDereferenceNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Procced Variable Dereference Node");
        throw std::runtime_error("VariableDereferenceNode::generateCode should be handled by parent.");        
    }

    std::unique_ptr<Node> VariableDereferenceNode::clone() const {
        auto clonedNode = std::make_unique<VariableDereferenceNode>();
        clonedNode->dereferenceType = dereferenceType;
    
        if (dereferenceType == DereferenceAccessType::VARIABLE_ACCESS && identifierAccessNode) {
            clonedNode->identifierAccessNode = std::unique_ptr<IdentifierNode>(
                dynamic_cast<IdentifierNode*>(identifierAccessNode->clone().release())
            );
        } else if (dereferenceType == DereferenceAccessType::ARRAY_ACCESS && arrayAccessNode) {
            clonedNode->arrayAccessNode = std::unique_ptr<ArrayAccessNode>(
                dynamic_cast<ArrayAccessNode*>(arrayAccessNode->clone().release())
            );
        }
    
        return clonedNode;
    }
}
