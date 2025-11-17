#ifndef LYNX_TYPE_NUMERIC_INSTRUCTION_HPP
#define LYNX_TYPE_NUMERIC_INSTRUCTION_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicsX86.h>
#include <limits>
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes::helper {

    inline llvm::Function* callOfIntrinsic(llvm::Module* module, llvm::Intrinsic::ID id, llvm::Type* type) {
        return llvm::Intrinsic::getOrInsertDeclaration(module, id, {type});
    }

    inline llvm::Value* intToFloat(const StrategyContext& stgCtx, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        llvm::Type* floatTy = stgCtx.instance->getType()->isIntegerTy(64)
                                ? builder.getDoubleTy()
                                : builder.getFloatTy();

        if (isSigned) return builder.CreateSIToFP(stgCtx.instance, floatTy);
        return builder.CreateUIToFP(stgCtx.instance, floatTy);
    }

    inline llvm::Value* intToFloat(const StrategyContext& stgCtx, llvm::Value* val, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        llvm::Type* floatTy = val->getType()->isIntegerTy(64)
                                ? builder.getDoubleTy()
                                : builder.getFloatTy();

        if (isSigned) return builder.CreateSIToFP(val, floatTy);
        return builder.CreateUIToFP(val, floatTy);
    }

    inline llvm::Value* floatToInt(const StrategyContext& stgCtx, llvm::Value* val, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        llvm::Type* intTy = stgCtx.instance->getType();

        if (isSigned) return builder.CreateFPToSI(val, intTy);
        return builder.CreateFPToUI(val, intTy);
    }

    inline llvm::Value* callOfUnaryIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID id, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        auto* floatVal = intToFloat(stgCtx, isSigned);
        auto* mod = stgCtx.ctx.getModule();
        auto* fun = helper::callOfIntrinsic(mod, id, floatVal->getType());
        auto* result = builder.CreateCall(fun, {floatVal});
        return floatToInt(stgCtx, result, isSigned);
    }

    inline llvm::Value* callOfBinaryIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID id, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        auto* lhs = intToFloat(stgCtx, isSigned);
        auto* rhs = intToFloat(stgCtx, stgCtx.args[0]);
        auto* mod = stgCtx.ctx.getModule();
        auto* fun = helper::callOfIntrinsic(mod, id, lhs->getType());
        auto* result = builder.CreateCall(fun, {lhs, rhs});
        return floatToInt(stgCtx, result, isSigned);
    }

    inline llvm::Value* callOfUnaryIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID id) {
        auto* mod = stgCtx.ctx.getModule();
        auto* func = callOfIntrinsic(mod, id, stgCtx.instance->getType());
        return stgCtx.ctx.getBuilder().CreateCall(func, { stgCtx.instance });
    }

    inline llvm::Value* callOfBinaryIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID id) {
        auto* mod = stgCtx.ctx.getModule();
        auto* func = helper::callOfIntrinsic(mod, id, stgCtx.instance->getType());
        return stgCtx.ctx.getBuilder().CreateCall(func, { stgCtx.instance, stgCtx.args[0] });
    }
}

#endif
