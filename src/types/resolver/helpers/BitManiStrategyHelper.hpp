#ifndef LYNX_TYPE_BITMANI_STRATEGY_HELPER_HPP
#define LYNX_TYPE_BITMANI_STRATEGY_HELPER_HPP

#include <llvm/IR/Value.h>

namespace LynxTypes::helper {

    // ------------------------------------------------------------------------
    // Rotate left helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* rotateLeft(BuilderT& builder, llvm::Value* val, llvm::Value* shift) {
        llvm::Value* width = llvm::ConstantInt::get(val->getType(), val->getType()->getIntegerBitWidth());
        llvm::Value* lshift = builder.CreateShl(val, shift);
        llvm::Value* rshift = builder.CreateLShr(val, builder.CreateSub(width, shift));
        return builder.CreateOr(lshift, rshift);
    }

    // ------------------------------------------------------------------------
    // Rotate right helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* rotateRight(BuilderT& builder, llvm::Value* val, llvm::Value* shift) {
        llvm::Value* width = llvm::ConstantInt::get(val->getType(), val->getType()->getIntegerBitWidth());
        llvm::Value* rshift = builder.CreateLShr(val, shift);
        llvm::Value* lshift = builder.CreateShl(val, builder.CreateSub(width, shift));
        return builder.CreateOr(lshift, rshift);
    }

    // ------------------------------------------------------------------------
    // Pop count helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* popCount(BuilderT& builder, llvm::Value* val) {
        llvm::Function* fn = llvm::Intrinsic::getOrInsertDeclaration(
            builder.GetInsertBlock()->getModule(),
            llvm::Intrinsic::ctpop,
            val->getType()
        );
        return builder.CreateCall(fn, val);
    }

    // ------------------------------------------------------------------------
    // Count leading zeros helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* countLeadingZeros(BuilderT& builder, llvm::Value* val) {
        llvm::Function* fn = llvm::Intrinsic::getOrInsertDeclaration(
            builder.GetInsertBlock()->getModule(),
            llvm::Intrinsic::ctlz,
            val->getType()
        );
        llvm::Value* isZeroUndef = llvm::ConstantInt::getFalse(builder.getContext());
        return builder.CreateCall(fn, {val, isZeroUndef});
    }

    // ------------------------------------------------------------------------
    // Count trailing zeros helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* countTrailingZeros(BuilderT& builder, llvm::Value* val) {
        llvm::Function* fn = llvm::Intrinsic::getOrInsertDeclaration(
            builder.GetInsertBlock()->getModule(),
            llvm::Intrinsic::cttz,
            val->getType()
        );
        llvm::Value* isZeroUndef = llvm::ConstantInt::getFalse(builder.getContext());
        return builder.CreateCall(fn, {val, isZeroUndef});
    }

    // ------------------------------------------------------------------------
    // Bit reversal helper
    // ------------------------------------------------------------------------
    template<typename BuilderT>
    inline llvm::Value* bitReverse(BuilderT& builder, llvm::Value* val) {
        llvm::Function* fn = llvm::Intrinsic::getOrInsertDeclaration(
            builder.GetInsertBlock()->getModule(),
            llvm::Intrinsic::bitreverse,
            val->getType()
        );
        return builder.CreateCall(fn, val);
    }
}

#endif
