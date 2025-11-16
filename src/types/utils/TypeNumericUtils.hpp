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
    constexpr inline llvm::Value* intMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::max(), true); 
    }

    constexpr inline llvm::Value* intMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::min(), true); 
    }

    constexpr inline llvm::Value* shortMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::max(), true); 
    }

    constexpr inline llvm::Value* shortMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::min(), true); 
    }

    constexpr inline llvm::Value* longMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::max(), true); 
    }

    constexpr inline llvm::Value* longMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::min(), true); 
    }

    constexpr inline llvm::Value* charMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::max(), true); 
    }

    constexpr inline llvm::Value* charMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::min(), true); 
    }

    constexpr inline llvm::Value* uintMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<unsigned int>::max(), false); 
    }

    // ======================================================
    // FLOATING-POINT CONSTANTS
    // ======================================================
    constexpr inline llvm::Value* floatMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::max());
    }

    constexpr inline llvm::Value* floatMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::lowest());
    }

    constexpr inline llvm::Value* doubleMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::max());
    }

    constexpr inline llvm::Value* doubleMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::lowest());
    }

    constexpr inline llvm::Value* floatEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::epsilon());
    }

    constexpr inline llvm::Value* doubleEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::epsilon());
    }

    // ======================================================
    // SPECIAL FLOATING-POINT VALUES
    // ======================================================
    constexpr inline llvm::Value* floatPosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), false));
    }

    constexpr inline llvm::Value* floatNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), true));
    }

    constexpr inline llvm::Value* doublePosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), false));
    }

    constexpr inline llvm::Value* doubleNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), true));
    }

    constexpr inline llvm::Value* floatNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEsingle()));
    }

    constexpr inline llvm::Value* doubleNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble()));
    }

    constexpr inline llvm::Value* floatZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0f));
    }

    constexpr inline llvm::Value* floatNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0f));
    }

    constexpr inline llvm::Value* doubleZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0));
    }

    constexpr inline llvm::Value* doubleNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0));
    }

    // ======================================================
    // GENERIC CONSTANTS (dynamic by type)
    // ======================================================
    constexpr inline llvm::Value* zero(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 0.0)
                                         : llvm::ConstantInt::get(type, 0);
    }

    constexpr inline llvm::Value* one(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 1.0)
                                         : llvm::ConstantInt::get(type, 1);
    }

    constexpr inline llvm::Value* negativeOne(llvm::LLVMContext& ctx, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, -1.0)
                                         : llvm::ConstantInt::get(type, -1, true);
    }

    // ======================================================
    // TYPE TRAITS
    // ======================================================
    constexpr inline llvm::Value* bitsOf(llvm::LLVMContext& ctx, llvm::Type* type) {
        if (type->isIntegerTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), type->getIntegerBitWidth());
        if (type->isFloatTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32);
        if (type->isDoubleTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64);
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0);
    }

    constexpr inline llvm::Value* isSigned(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    constexpr inline llvm::Value* isInteger(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    constexpr inline llvm::Value* isFloat(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isFloatingPointTy());
    }

    // ======================================================
    // COMMON BOOLEAN & MISC CONSTANTS
    // ======================================================
    constexpr inline llvm::Value* allOnesInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ~0u);
    }

    constexpr inline llvm::Value* halfMaxInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<int32_t>::max() / 2);
    }

    constexpr inline llvm::Value* minPositiveFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::min());
    }

    constexpr inline llvm::Value* mantissaBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<float>::digits);
    }

    constexpr inline llvm::Value* exponentBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<float>::max_exponent - std::numeric_limits<float>::min_exponent + 1);
    }

    constexpr inline llvm::Value* isFiniteFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::getTrue(ctx);
    }

}

#endif
