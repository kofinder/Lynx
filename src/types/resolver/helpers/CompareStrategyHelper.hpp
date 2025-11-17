#ifndef LYNX_TYPE_COMPARE_STRATEGY_HELPER_HPP
#define LYNX_TYPE_COMPARE_STRATEGY_HELPER_HPP

#include <llvm/IR/Value.h>

namespace LynxTypes::helper {

    // ==========================
    // EQ Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareEq(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpOEQ(lhs, rhs) : builder.CreateICmpEQ(lhs, rhs);
    }

    // ==========================
    // NE Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareNe(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpONE(lhs, rhs) : builder.CreateICmpNE(lhs, rhs);
    }

    // ==========================
    // LT Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareLt(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpOLT(lhs, rhs)  : builder.CreateICmpSLT(lhs, rhs);
    }

    // ==========================
    // LE Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareLe(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpOLE(lhs, rhs)  : builder.CreateICmpSLE(lhs, rhs);
    }

    // ==========================
    // GT Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareGt(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpOGT(lhs, rhs)  : builder.CreateICmpSGT(lhs, rhs);
    }

    // ==========================
    // GE Helper
    // ==========================
    template<typename BuilderT, typename ValT>
    inline llvm::Value* compareGe(BuilderT& builder, ValT* lhs, ValT* rhs, bool isFloat) {
        return isFloat ? builder.CreateFCmpOGE(lhs, rhs)  : builder.CreateICmpSGE(lhs, rhs);
    }
}

#endif
