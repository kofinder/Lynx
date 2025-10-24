#ifndef LYNX_FILE_UTILS_HPP
#define LYNX_FILE_UTILS_HPP


#include <iostream>
#include <string>
#include <vector>
#include <optional>

#include "LiteralNode.hpp"
#include "PrimaryExpressionNode.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/DerivedTypes.h>

#include <constants/OperatorType.hpp>
#include <context/AstContext.hpp>


namespace LynxAst {
    
    using namespace LynxTypes;
    using namespace LynxContext;
    using namespace LynxConstants;
    
    llvm::Value* castToCString(std::shared_ptr<AstContext> context, llvm::Value* val) {
        auto& builder = context->getBuilder();
        if (val->getType()->isPointerTy() && val->getType()->getPointerElementType()->isIntegerTy(8)) {
            return val; 
        }
        return builder.CreateBitCast(val, llvm::Type::getInt8PtrTy(context->getLLVMContext()), "castToI8Ptr");
    }

    std::optional<std::string> extractStringLiteral(const ExpressionNode& node) {
        if (auto primary = dynamic_cast<const PrimaryExpressionNode*>(&node)) {
            if (auto literal = dynamic_cast<const LiteralNode*>(primary->getInnerExpression())) {
                const auto& value = literal->getLiteralValue();
                if (std::holds_alternative<std::string>(value)) {
                    return std::get<std::string>(value);
                }
            }
        }
        return std::nullopt;
    }
    
}

#endif
