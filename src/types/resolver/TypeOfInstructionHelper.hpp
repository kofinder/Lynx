#ifndef LYNX_TYPE_NUMERIC_INSTRUCTION_HPP
#define LYNX_TYPE_NUMERIC_INSTRUCTION_HPP

#include <limits>
#include <bit>
#include <cstdint>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicsX86.h>
#include "tmpl/TypeNumericPromotion.hpp"
#include "TypeStrategyContext.hpp"
#include <constants/MagicNumericConstants.hpp>
#include <constants/metadata/MetadataTypeConstants.hpp>

namespace LynxTypes::helper {

    using namespace TypePromotion;
    using namespace MetadataTypeConstants;

    enum class ArithmeticOp : std::uint8_t { Add, Sub, Mul, Div, Mod };
    enum class BitwiseOp : std::uint8_t { And, Or, Xor, Shl, Shr, Not };
    enum class CompareOp : std::uint8_t { Eq, Ne, Lt, Le, Gt, Ge };
    struct IntScaleBits { unsigned value; };
    struct FloatScaleBits { unsigned value; };


    inline llvm::Function* callOfIntrinsic(llvm::Module* module, llvm::Intrinsic::ID instId, llvm::Type* type) noexcept {
        return llvm::Intrinsic::getOrInsertDeclaration(module, instId, { type });
    }

    inline llvm::Value* intToFloat(llvm::IRBuilder<>& builder, llvm::Value* value) noexcept {
        const auto* valueType = value->getType();
        if (valueType->isFloatingPointTy()) return value;
        llvm::Type* floatTy = valueType->isIntegerTy(BIT_WIDTH_LONG) ? builder.getDoubleTy() : builder.getFloatTy();
        return builder.CreateSIToFP(value, floatTy);
    }

    inline llvm::Value* callOfArithmeticIntrisic(const StrategyContext& stg, ArithmeticOp opr) noexcept {
        auto* lhs = stg.instance;
        auto* lhsPtr = stg.instancePtr;
        auto* rhs = stg.args[0];
        auto& builder = stg.ctx.getBuilder();

        auto promoted = promoteNumericOperands(lhs, rhs, builder);
        promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
        promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);

        llvm::Value* result = nullptr;
        if (promoted.isFloating) {
            switch (opr) {
                case ArithmeticOp::Add: result = builder.CreateFAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                case ArithmeticOp::Sub: result = builder.CreateFSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                case ArithmeticOp::Mul: result = builder.CreateFMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                case ArithmeticOp::Div: result = builder.CreateFDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                case ArithmeticOp::Mod: result = builder.CreateFRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
            }
        } else {
            switch (opr) {
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

    inline llvm::Value* callOfBitwiseIntrisic(const StrategyContext& stg, BitwiseOp opr) noexcept {
        auto& builder = stg.ctx.getBuilder();
        auto* lhs = stg.instance;
        auto* lhsPtr = stg.instancePtr;
        auto* rhs = opr == BitwiseOp::Not ? llvm::ConstantInt::get(builder.getInt32Ty(), 0) :  stg.args[0];

        auto promoted = promoteNumericOperands(lhs, rhs, builder);
        promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
        promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);

        llvm::Value* lhsVal = promoted.lhs;
        llvm::Value* rhsVal = promoted.rhs;
    
        if (promoted.isFloating) {
            const llvm::Type* type = promoted.commonType;
            llvm::Type* intTy = nullptr;
            if (type->isFloatTy()) {
                intTy = builder.getInt32Ty();
            } else  {
                intTy = builder.getInt64Ty();
            }
            lhsVal = builder.CreateBitCast(lhsVal, intTy, "bit.lhs");
            rhsVal = builder.CreateBitCast(rhsVal, intTy, "bit.rhs");
        } 

        llvm::Value* result = nullptr; // NOLINT(misc-const-correctness)
        switch (opr) {
            case BitwiseOp::And: result = builder.CreateAnd(lhsVal, rhsVal, llvm::Twine("bit.and")); break;
            case BitwiseOp::Or: result = builder.CreateOr(lhsVal, rhsVal, llvm::Twine("bit.or")); break;
            case BitwiseOp::Xor: result = builder.CreateXor(lhsVal, rhsVal, llvm::Twine("bit.xor")); break;
            case BitwiseOp::Shl: result = builder.CreateShl(lhsVal, rhsVal, llvm::Twine("bit.shl")); break;
            case BitwiseOp::Shr: result = builder.CreateLShr(lhsVal, rhsVal, llvm::Twine("bit.shr")); break;
            case BitwiseOp::Not: result = builder.CreateNot(lhsVal, llvm::Twine("bit.not")); break;
        }
        
        builder.CreateStore(result, lhsPtr);
        return result;
    }

    inline llvm::Value* callOfCompareIntrinsic(const StrategyContext& stg, CompareOp opr, bool isFloating = false) noexcept {
        auto* lhsVal = stg.instance;
        auto* rhsVal = stg.args[0];

        auto& builder = stg.ctx.getBuilder();
        llvm::Value* result = nullptr;
        if(isFloating) {
            switch (opr) {
                case CompareOp::Eq: result = builder.CreateFCmpOEQ(lhsVal, rhsVal, llvm::Twine("cmp.eq")); break;
                case CompareOp::Ne: result = builder.CreateFCmpONE(lhsVal, rhsVal, llvm::Twine("cmp.ne")); break;
                case CompareOp::Lt: result = builder.CreateFCmpOLT(lhsVal, rhsVal, llvm::Twine("cmp.lt")); break;
                case CompareOp::Le: result = builder.CreateFCmpOLE(lhsVal, rhsVal, llvm::Twine("cmp.le")); break;
                case CompareOp::Gt: result = builder.CreateFCmpOGT(lhsVal, rhsVal, llvm::Twine("cmp.gt")); break;
                case CompareOp::Ge: result = builder.CreateFCmpOGE(lhsVal, rhsVal, llvm::Twine("cmp.ge")); break;
            }
        } else {
            const bool isUnsigned = lhsVal->getType()->isIntegerTy() && !lhsVal->getType()->isIntegerTy(1);
            switch (opr) {
                case CompareOp::Eq: result = builder.CreateICmpEQ(lhsVal, rhsVal, llvm::Twine("cmp.eq")); break;
                case CompareOp::Ne: result = builder.CreateICmpNE(lhsVal, rhsVal, llvm::Twine("cmp.ne")); break;
                case CompareOp::Lt: result = isUnsigned 
                                     ? builder.CreateICmpULT(lhsVal, rhsVal, llvm::Twine("cmp.lt")) 
                                     : builder.CreateICmpSLT(lhsVal, rhsVal, llvm::Twine("cmp.lt")); break;
                case CompareOp::Le: result = isUnsigned 
                                     ? builder.CreateICmpULE(lhsVal, rhsVal, llvm::Twine("cmp.le")) 
                                     : builder.CreateICmpSLE(lhsVal, rhsVal, llvm::Twine("cmp.le")); break;
                case CompareOp::Gt: result = isUnsigned 
                                     ? builder.CreateICmpUGT(lhsVal, rhsVal, llvm::Twine("cmp.gt")) 
                                     : builder.CreateICmpSGT(lhsVal, rhsVal, llvm::Twine("cmp.gt")); break;
                case CompareOp::Ge: result = isUnsigned 
                                     ? builder.CreateICmpUGE(lhsVal, rhsVal, llvm::Twine("cmp.ge")) 
                                     : builder.CreateICmpSGE(lhsVal, rhsVal, llvm::Twine("cmp.ge")); break;
            }
        }

        return result;
    }

    inline llvm::Value* callOfMinxMaxPredict(const StrategyContext& stg, llvm::ICmpInst::Predicate pred) noexcept {
        auto* lhs = stg.instance;
        auto* rhs = stg.args[0];
        auto* lhsPtr = stg.instancePtr;

        auto& builder = stg.ctx.getBuilder();
        auto* cmp = builder.CreateICmp(pred, lhs, rhs);
        auto* result = builder.CreateSelect(cmp, lhs, rhs);
        builder.CreateStore(result, lhsPtr);
        return result;
    }

    inline llvm::Value* callOfMinxMaxInstrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId)  noexcept {
        auto* lhs = stg.instance;
        auto* rhs = stg.args[0];
        auto* lhsPtr = stg.instancePtr;

        auto& builder = stg.ctx.getBuilder();
        auto* module = stg.ctx.getModule();

        auto* func = callOfIntrinsic(module, instId, lhs->getType());
        auto* result = builder.CreateCall(func, {lhs, rhs});
        builder.CreateStore(result, lhsPtr);
        return result;
    }

    inline llvm::Value* callOfBinaryIntrinsic(const StrategyContext& stgCtx, llvm::Intrinsic::ID instId) noexcept {
        auto* mod = stgCtx.ctx.getModule();
        auto* func = callOfIntrinsic(mod, instId, stgCtx.instance->getType());
        return stgCtx.ctx.getBuilder().CreateCall(func, { stgCtx.instance, stgCtx.args[0] });
    }

    inline llvm::Value* callOfBitManiIntrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId) noexcept {
        auto* mod = stg.ctx.getModule();
        auto& builder = stg.ctx.getBuilder();

        auto* lhs = stg.instance;
        auto* type = stg.instance->getType();

        auto* func = callOfIntrinsic(mod, instId, type);

        if (instId == llvm::Intrinsic::ctpop ||
            instId == llvm::Intrinsic::bitreverse) {
            return builder.CreateCall(func, { lhs });
        }

        if (instId == llvm::Intrinsic::ctlz || instId == llvm::Intrinsic::cttz) {
            return builder.CreateCall(func, { lhs, llvm::ConstantInt::getFalse(builder.getContext()) });
        }

        if (instId == llvm::Intrinsic::fshl || instId == llvm::Intrinsic::fshr) {
            auto* rhs = stg.args[0];
            return builder.CreateCall(func, { lhs, lhs, rhs }); // (a=x, b=x, sh)
        }

        return nullptr;
    }

    inline llvm::Value* callOfFixedPointIntrinsic(
        const StrategyContext& stg,
        llvm::Intrinsic::ID instId,
        IntScaleBits intScale = { BIT_WIDTH_BYTE },
        FloatScaleBits floatScale = { BIT_WIDTH_SHORT }) noexcept {
        auto& builder = stg.ctx.getBuilder();
        auto* mod = stg.ctx.getModule();
        llvm::Value* lhs = stg.instance;
        llvm::Type* lhsTy = lhs->getType();

        // -------------------------------------------------------------------------
        // Integer fixed-point intrinsic path
        // -------------------------------------------------------------------------
        if (!lhsTy->isFloatingPointTy()) {
            const unsigned bitWidth = lhsTy->getIntegerBitWidth();
            const unsigned scaleBits = intScale.value;
            if (scaleBits > bitWidth) llvm::report_fatal_error("scaleBits too large for integer operand width");
            auto* scaleConst = llvm::ConstantInt::get(lhsTy, llvm::APInt(bitWidth, scaleBits));
            auto* func = callOfIntrinsic(mod, instId, lhsTy);
            return builder.CreateCall(func, { lhs, lhs, scaleConst });
        }
            
        // -------------------------------------------------------------------------
        // Floating-point emulated fixed-point
        // -------------------------------------------------------------------------
        const unsigned scaleBits = floatScale.value;
        const auto scale = double(1ULL << scaleBits);
        auto* scaleConst = llvm::ConstantFP::get(lhsTy, scale);
        auto* func = callOfIntrinsic(mod, instId, lhsTy);
        return builder.CreateCall(func, { lhs, lhs, scaleConst });
    }

    inline llvm::Value* callOfOverflowIntrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId) {
        auto* mod = stg.ctx.getModule();
        auto& builder = stg.ctx.getBuilder();

        auto* lhs = stg.instance;
        auto* rhs = stg.args[0];

        auto* func = callOfIntrinsic(mod, instId, lhs->getType());
        auto* ovf = builder.CreateCall(func, { lhs, rhs }, llvm::Twine("inst.overflow"));

        auto* result = builder.CreateExtractValue(ovf, 0, "result"); // integer
        // auto* status = builder.CreateExtractValue(ovf, 1, "status"); // boolean

        return result;
    }
    
        
    inline llvm::Value* callOfSaturationIntrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId) noexcept {
        auto* mod = stg.ctx.getModule();
        auto& builder = stg.ctx.getBuilder();

        auto* lhs = stg.instance;
        auto* rhs = stg.args[0];

        auto* func = callOfIntrinsic(mod, instId, lhs->getType());
        auto* result = builder.CreateCall(func, { lhs, rhs }, llvm::Twine("inst.saturation"));
        return result;
    }

    inline llvm::Value* intToFloat(const StrategyContext& stgCtx, llvm::Value* val, bool isSigned) noexcept {
        auto& builder = stgCtx.ctx.getBuilder();
        llvm::Type* floatTy = val->getType()->isIntegerTy(BIT_WIDTH_LONG)
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

    inline llvm::Value* callOfMathUnaryIntrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId) noexcept {
        auto* mod = stg.ctx.getModule();
        auto& builder = stg.ctx.getBuilder();
        auto* lhs = intToFloat(builder, stg.instance);
        auto* func = callOfIntrinsic(mod, instId, lhs->getType());
        auto* result = builder.CreateCall(func, {lhs});
        return result;
    }   
    
    inline llvm::Value* callOfMathBinaryIntrinsic(const StrategyContext& stg, llvm::Intrinsic::ID instId) noexcept {
        auto* mod = stg.ctx.getModule();
        auto& builder = stg.ctx.getBuilder();
        auto* lhs = intToFloat(builder, stg.instance);
        auto* rhs = intToFloat(builder, stg.args[0]);
        auto* func = callOfIntrinsic(mod, instId, lhs->getType());
        auto* result = builder.CreateCall(func, {lhs, rhs});
        return result;
    }    
}

#endif
