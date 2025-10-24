#include "ObjectAccessNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxContext;


namespace LynxAst {
    llvm::Value* ObjectAccessNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Generating Object Access Node ............{}", fieldName);

        llvm::Value* result = nullptr;
        if (nextAccessNode) {
            result = nextAccessNode->generateCode(astContext); 
        }
    
        return result;
    }
}