#include "LiteralNode.hpp"
#include <types/interfaces/BaseType.hpp>


namespace LynxAst {

    using namespace LynxTypes;

    llvm::Value* LiteralNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Emit IR code ....");

        if(isNull()) {
            auto& context = astContext->getLLVMContext();
            auto* int32PtrType = llvm::Type::getInt32PtrTy(context); // i32*
            auto* nullPtr = llvm::ConstantPointerNull::get(int32PtrType);
            return nullPtr;
        }

        auto variableType = astContext->findType(valueType);

        if (!variableType) {
            LOG_ERROR("Failed to find type for literal");
            return nullptr;
        }
    
        return variableType->createValue(literalData);
    }

    std::unique_ptr<Node> LiteralNode::clone() const {
        return std::make_unique<LiteralNode>(*this);
    }
}
