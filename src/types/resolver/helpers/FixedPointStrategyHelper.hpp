#ifndef LYNX_TYPE_FIXED_POINT_STRATEGY_HELPER_HPP
#define LYNX_TYPE_FIXED_POINT_STRATEGY_HELPER_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Constants.h>
#include <cmath>
#include <type_traits>
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes::helper {

    inline llvm::IntegerType* pickIntForFP(llvm::Type* fpType, llvm::LLVMContext& C) {
        if (fpType->isDoubleTy()) return llvm::Type::getInt64Ty(C);
        return llvm::Type::getInt32Ty(C);
    }

    // ============================================================
    // Signed/Unsigned Fixed Intrinsic Call (integer inputs)
    // ============================================================
    inline llvm::Value* callFixedIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID id) {
        llvm::Value* lhs = stgCtx.instance;
        llvm::Value* rhs = stgCtx.args.at(0);
        auto& builder = stgCtx.ctx.getBuilder();
        auto* module = stgCtx.ctx.getModule();

        auto* fn = llvm::Intrinsic::getOrInsertDeclaration(module, id, { lhs->getType() });
        llvm::Value* scaleConst = llvm::ConstantInt::get(builder.getInt32Ty(), 16);
        return builder.CreateCall(fn, { lhs, rhs, scaleConst });
    }

    // ============================================================
    // FP → FixedInt (scaled by 2^Scale)
    // ============================================================
    inline llvm::Value* floatToFixedInt(const StrategyContext& stgCtx, llvm::Value* fpVal, llvm::IntegerType* outIntTy, bool isUnsigned) {
        const double scaleFactor = std::ldexp(1.0, 8);
        auto* scaleFP = llvm::ConstantFP::get(fpVal->getType(), scaleFactor);

        auto& builder = stgCtx.ctx.getBuilder();
        auto* scaled = builder.CreateFMul(fpVal, scaleFP);
        return isUnsigned
               ? builder.CreateFPToUI(scaled, outIntTy)
               : builder.CreateFPToSI(scaled, outIntTy);
    }

    // ============================================================
    // FixedInt → FP (divide by 2^Scale)
    // ============================================================
    inline llvm::Value* fixedIntToFloat(const StrategyContext& stgCtx, llvm::Value* intVal, llvm::Type* fpTy, bool isUnsigned) {
        auto& builder = stgCtx.ctx.getBuilder();
        auto* fp = isUnsigned ? builder.CreateUIToFP(intVal, fpTy)
                       : builder.CreateSIToFP(intVal, fpTy);

        const double scaleFactor = std::ldexp(1.0, 8);
        llvm::Constant* denom = llvm::ConstantFP::get(fpTy, scaleFactor);
        return builder.CreateFDiv(fp, denom);
    }

    // ============================================================
    // FP inputs → Fixed intrinsic → FP output
    // ============================================================
    inline llvm::Value* callIntIntrinsicFromFP(const StrategyContext& stgCtx, llvm::Intrinsic::ID id, bool isUnsigned) {

        llvm::Value* lhsFP = stgCtx.instance;
        llvm::Value* rhsFP = stgCtx.args.at(0);
        llvm::Type* fpTy = lhsFP->getType();

        llvm::IntegerType* intTy = pickIntForFP(fpTy, fpTy->getContext());

        auto* lhsInt = floatToFixedInt(stgCtx, lhsFP, intTy, isUnsigned);
        auto* rhsInt = floatToFixedInt(stgCtx, rhsFP, intTy, isUnsigned);

        auto& builder = stgCtx.ctx.getBuilder();
        auto* module = stgCtx.ctx.getModule();

        auto* fn = llvm::Intrinsic::getOrInsertDeclaration(module, id, { intTy });
        llvm::Value* scaleConst = llvm::ConstantInt::get(builder.getInt32Ty(), 16);
        auto* intRes = builder.CreateCall(fn, { lhsInt, rhsInt, scaleConst });
        return fixedIntToFloat(stgCtx, intRes, fpTy, isUnsigned);
    }
}

#endif
