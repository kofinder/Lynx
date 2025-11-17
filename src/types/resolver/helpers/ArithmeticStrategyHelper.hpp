#ifndef LYNX_TYPE_ARITHMETIC_STRATEGY_HELPER_HPP
#define LYNX_TYPE_ARITHMETIC_STRATEGY_HELPER_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicsX86.h>
#include "tmpl/TypeNumericPromotion.hpp"
#include "resolver/TypeStrategyContext.hpp"
#include "utils/TypeOperandsValidationUtils.hpp"
#include <constants/metadata/MetadataTypeConstants.hpp>

namespace LynxTypes::helper {

    using namespace TypePromotion;
    using namespace MetadataTypeConstants;

    enum class ArithmeticOp { Add, Sub, Mul, Div, Mod };

    inline constexpr llvm::Value* generateBinaryOp(const StrategyContext& stgCtx, ArithmeticOp op) noexcept {
        auto* lhs = stgCtx.instance;
        auto* lhsPtr = stgCtx.instancePtr;
        auto* rhs = stgCtx.args[0];
        auto& builder = stgCtx.ctx.getBuilder();

        if (!validateOperands(lhs, lhsPtr, rhs)) return nullptr;

        auto promoted = promoteNumericOperands(lhs, rhs, builder);
        promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
        promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);

        llvm::Value* result = nullptr;
        if (promoted.isFloating) {
            switch (op) {
                case ArithmeticOp::Add: result = builder.CreateFAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                case ArithmeticOp::Sub: result = builder.CreateFSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                case ArithmeticOp::Mul: result = builder.CreateFMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                case ArithmeticOp::Div: result = builder.CreateFDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                case ArithmeticOp::Mod: result = builder.CreateFRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
            }
        } else {
            switch (op) {
                case ArithmeticOp::Add: result = builder.CreateAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                case ArithmeticOp::Sub: result = builder.CreateSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                case ArithmeticOp::Mul: result = builder.CreateMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                case ArithmeticOp::Div: result = builder.CreateSDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                case ArithmeticOp::Mod: result = builder.CreateSRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
            }
        }

        builder.CreateStore(result, lhsPtr);
        return result;
    }
    
}

#endif
