#ifndef LYNX_TYPE_CONSTANTS_HPP
#define LYNX_TYPE_CONSTANTS_HPP

#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/ADT/APFloat.h>
#include <limits>

namespace LynxTypes {

    // ======================================================
    // INTEGER CONSTANTS
    // ======================================================
    inline llvm::Value* intMax(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::max(), true); 
    }

    inline llvm::Value* intMin(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::min(), true); 
    }

    inline llvm::Value* shortMax(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::max(), true); 
    }

    inline llvm::Value* shortMin(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::min(), true); 
    }

    inline llvm::Value* longMax(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::max(), true); 
    }

    inline llvm::Value* longMin(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::min(), true); 
    }

    inline llvm::Value* charMax(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::max(), true); 
    }

    inline llvm::Value* charMin(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::min(), true); 
    }

    inline llvm::Value* uintMax(llvm::LLVMContext& ctx) { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<unsigned int>::max(), false); 
    }

    // ======================================================
    // FLOATING-POINT CONSTANTS
    // ======================================================
    inline llvm::Value* floatMax(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::max());
    }

    inline llvm::Value* floatMin(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::lowest());
    }

    inline llvm::Value* doubleMax(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::max());
    }

    inline llvm::Value* doubleMin(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::lowest());
    }

    inline llvm::Value* floatEpsilon(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::epsilon());
    }

    inline llvm::Value* doubleEpsilon(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::epsilon());
    }

    // ======================================================
    // SPECIAL FLOATING-POINT VALUES
    // ======================================================
    inline llvm::Value* floatPosInfinity(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), false));
    }

    inline llvm::Value* floatNegInfinity(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), true));
    }

    inline llvm::Value* doublePosInfinity(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), false));
    }

    inline llvm::Value* doubleNegInfinity(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), true));
    }

    inline llvm::Value* floatNaN(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEsingle()));
    }

    inline llvm::Value* doubleNaN(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble()));
    }

    inline llvm::Value* floatZero(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0f));
    }

    inline llvm::Value* floatNegZero(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0f));
    }

    inline llvm::Value* doubleZero(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0));
    }

    inline llvm::Value* doubleNegZero(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0));
    }

    // ======================================================
    // GENERIC CONSTANTS (dynamic by type)
    // ======================================================
    inline llvm::Value* zero(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 0.0)
                                         : llvm::ConstantInt::get(type, 0);
    }

    inline llvm::Value* one(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 1.0)
                                         : llvm::ConstantInt::get(type, 1);
    }

    inline llvm::Value* negativeOne(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, -1.0)
                                         : llvm::ConstantInt::get(type, -1, true);
    }

    // ======================================================
    // TYPE TRAITS
    // ======================================================
    inline llvm::Value* bitsOf(llvm::LLVMContext& ctx, llvm::Type* type) {
        if (type->isIntegerTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), type->getIntegerBitWidth());
        if (type->isFloatTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32);
        if (type->isDoubleTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64);
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0);
    }

    inline llvm::Value* isSigned(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    inline llvm::Value* isInteger(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    inline llvm::Value* isFloat(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isFloatingPointTy());
    }

    // ======================================================
    // COMMON BOOLEAN & MISC CONSTANTS
    // ======================================================
    inline llvm::Value* allOnesInt32(llvm::LLVMContext& ctx) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ~0u);
    }

    inline llvm::Value* halfMaxInt32(llvm::LLVMContext& ctx) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<int32_t>::max() / 2);
    }

    inline llvm::Value* minPositiveFloat(llvm::LLVMContext& ctx) {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::min());
    }

    inline llvm::Value* mantissaBitsFloat(llvm::LLVMContext& ctx) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<float>::digits);
    }

    inline llvm::Value* exponentBitsFloat(llvm::LLVMContext& ctx) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<float>::max_exponent - std::numeric_limits<float>::min_exponent + 1);
    }

    inline llvm::Value* isFiniteFloat(llvm::LLVMContext& ctx) {
        return llvm::ConstantInt::getTrue(ctx);
    }

}

#endif
