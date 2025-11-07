/**
 * @file FileUtils.hpp
 * @brief Utility functions for file and string handling during LLVM code generation.
 * 
 * The FileUtils namespace provides helper functions for string literal extraction,
 * type casting to C-style strings, and other basic file-related operations in the AST.
 * 
 * **Key Responsibilities:**
 * - Cast LLVM values to `i8*` (C-style string pointers) for code generation.
 * - Extract string literals from expression nodes safely.
 * 
 * **Used By:**
 * - Code generation routines that require constant string handling.
 * 
 * @see AstContext, ExpressionNode, LiteralNode, PrimaryExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

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
