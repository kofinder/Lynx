#ifndef LYNX_TYPE_ABS_STRATEGY_HELPER_HPP
#define LYNX_TYPE_ABS_STRATEGY_HELPER_HPP

#include <llvm/IR/Value.h>

namespace LynxTypes::helper {

    // ==========================
    // Sign Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* selectSign(BuilderT& builder, ValT* val, bool isFloating) {
        llvm::Type* type = val->getType();

        if (isFloating) {
            auto* zero   = llvm::ConstantFP::get(type, 0.0);
            auto* one    = llvm::ConstantFP::get(type, 1.0);
            auto* negOne = llvm::ConstantFP::get(type, -1.0);

            auto* isPos = builder.CreateFCmpOGT(val, zero);
            auto* isNeg = builder.CreateFCmpOLT(val, zero);

            auto* result = builder.CreateSelect(isPos, one, zero);
            return builder.CreateSelect(isNeg, negOne, result);
        } else {
            auto* zero   = llvm::ConstantInt::get(type, 0);
            auto* one    = llvm::ConstantInt::get(type, 1);
            auto* negOne = llvm::ConstantInt::get(type, -1);

            auto* isPos = builder.CreateICmpSGT(val, zero);
            auto* isNeg = builder.CreateICmpSLT(val, zero);

            auto* result = builder.CreateSelect(isPos, one, zero);
            return builder.CreateSelect(isNeg, negOne, result);
        }
    }

    // ==========================
    // clamp Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* clampHelper(BuilderT& builder, ValT* val, ValT* minVal, ValT* maxVal, bool isFloating) {
        if (isFloating) {
            return builder.CreateSelect(
                builder.CreateFCmpOLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateFCmpOGT(val, maxVal), maxVal, val)
            );
        } else {
            return builder.CreateSelect(
                builder.CreateICmpSLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateICmpSGT(val, maxVal), maxVal, val)
            );
        }
    }

    // ==========================
    // isEven Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* isEvenHelper(BuilderT& builder, ValT* val, bool isFloating) {
        llvm::Type* type = val->getType();
        if (isFloating) {
            auto* two  = llvm::ConstantFP::get(type, 2.0);
            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* rem  = builder.CreateFRem(val, two);
            return builder.CreateFCmpUEQ(rem, zero);
        } else {
            auto* two  = llvm::ConstantInt::get(type, 2);
            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* rem  = builder.CreateURem(val, two);
            return builder.CreateICmpEQ(rem, zero);
        }
    }

    // ==========================
    // isOdd Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* isOddHelper(BuilderT& builder, ValT* val, bool isFloating) {
        llvm::Type* type = val->getType();
        if (isFloating) {
            auto* two  = llvm::ConstantFP::get(type, 2.0);
            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* rem  = builder.CreateFRem(val, two);
            return builder.CreateFCmpUNE(rem, zero);
        } else {
            auto* two  = llvm::ConstantInt::get(type, 2);
            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* rem  = builder.CreateURem(val, two);
            return builder.CreateICmpNE(rem, zero);
        }
    }
}

#endif
