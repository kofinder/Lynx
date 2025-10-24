#include "LiteralNode.hpp"
#include <string>
#include <types/interfaces/BaseType.hpp>

using namespace LynxTypes;

namespace LynxAst {
    
    llvm::Value* LiteralNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked ....");

        if(isNull())  {
            auto& context = astContext->getLLVMContext();
            auto* int32PtrType = llvm::Type::getInt32PtrTy(context); // i32*
            auto* nullPtr = llvm::ConstantPointerNull::get(int32PtrType);
            return nullPtr;
        }

        auto variableType = astContext->findType(valueType);
        return variableType->createValue(literalData);
    }

    std::unique_ptr<Node> LiteralNode::clone() const {
        LOG_WARN("[LiteralNode::clone] Cloning literal of type {} with value", static_cast<int>(valueType));
        return std::make_unique<LiteralNode>(*this);
    }
}
