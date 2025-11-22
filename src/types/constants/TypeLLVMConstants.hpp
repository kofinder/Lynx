/**
 * @file TypeLLVMConstants.hpp
 * @brief Provides factory utilities for generating LLVM constant values used
 *        throughout the Lynx type system.
 *
 * This header centralizes the creation of LLVM IR constants for integers,
 * floating-point values, NaN/Infinity, dynamic type-dependent constants,
 * and type-trait metadata such as bit-width and signedness flags.
 *
 * Consolidating these helpers ensures:
 *  - consistency across the entire codebase,
 *  - reduced duplication when generating LLVM constants,
 *  - improved readability and maintainability,
 *  - safer handling of floating-point special values.
 *
 * These functions are implemented as `constexpr inline` to allow
 * constant-folding where possible and avoid ODR violations across translation units.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_TYPE_LLVM_CONSTANTS_HPP
#define LYNX_TYPE_LLVM_CONSTANTS_HPP

#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/ADT/APFloat.h>
#include <limits>

namespace LynxTypes {

    constexpr unsigned SHORT_BIT_WIDTH  = 16;
    constexpr unsigned INT_BIT_WIDTH   = 32;
    constexpr unsigned LONG_BIT_WIDTH  = 64;



    // ======================================================
    // INTEGER CONSTANTS
    // ======================================================
    constexpr llvm::Value* intMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::max(), true); 
    }

    constexpr llvm::Value* intMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::min(), true); 
    }

    constexpr llvm::Value* shortMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::max(), true); 
    }

    constexpr llvm::Value* shortMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::min(), true); 
    }

    constexpr llvm::Value* longMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::max(), true); 
    }

    constexpr llvm::Value* longMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::min(), true); 
    }

    constexpr llvm::Value* charMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::max(), true); 
    }

    constexpr llvm::Value* charMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::min(), true); 
    }

    constexpr llvm::Value* uintMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<unsigned int>::max(), false); 
    }

    // ======================================================
    // FLOATING-POINT CONSTANTS
    // ======================================================
    constexpr llvm::Value* floatMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::max());
    }

    constexpr llvm::Value* floatMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::lowest());
    }

    constexpr llvm::Value* doubleMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::max());
    }

    constexpr llvm::Value* doubleMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::lowest());
    }

    constexpr llvm::Value* floatEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::epsilon());
    }

    constexpr llvm::Value* doubleEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::epsilon());
    }

    // ======================================================
    // SPECIAL FLOATING-POINT VALUES
    // ======================================================
    constexpr llvm::Value* floatPosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), false));
    }

    constexpr llvm::Value* floatNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), true));
    }

    constexpr llvm::Value* doublePosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), false));
    }

    constexpr llvm::Value* doubleNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), true));
    }

    constexpr llvm::Value* floatNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEsingle()));
    }

    constexpr llvm::Value* doubleNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble()));
    }

    constexpr llvm::Value* floatZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0F));
    }

    constexpr llvm::Value* floatNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0F));
    }

    constexpr llvm::Value* doubleZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0));
    }

    constexpr llvm::Value* doubleNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0));
    }

    // ======================================================
    // GENERIC CONSTANTS (dynamic by type)
    // ======================================================
    constexpr llvm::Value* zero(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 0.0)
                                         : llvm::ConstantInt::get(type, 0);
    }

    constexpr llvm::Value* one(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 1.0)
                                         : llvm::ConstantInt::get(type, 1);
    }

    constexpr llvm::Value* negativeOne(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, -1.0)
                                         : llvm::ConstantInt::get(type, -1, true);
    }

    // ======================================================
    // TYPE TRAITS
    // ======================================================
    constexpr llvm::Value* bitsOf(llvm::LLVMContext& ctx, llvm::Type* type) {
        if (type->isIntegerTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), type->getIntegerBitWidth());
        if (type->isFloatTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), INT_BIT_WIDTH);
        if (type->isDoubleTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), LONG_BIT_WIDTH);
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0);
    }

    constexpr llvm::Value* isSigned(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    constexpr llvm::Value* isInteger(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isIntegerTy());
    }

    constexpr llvm::Value* isFloat(llvm::LLVMContext& ctx, llvm::Type* type) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), type->isFloatingPointTy());
    }

    // ======================================================
    // COMMON BOOLEAN & MISC CONSTANTS
    // ======================================================
    constexpr llvm::Value* allOnesInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ~0U);
    }

    constexpr llvm::Value* halfMaxInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<int32_t>::max() / 2);
    }

    constexpr llvm::Value* minPositiveFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::min());
    }

    constexpr llvm::Value* mantissaBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<float>::digits);
    }

    constexpr llvm::Value* exponentBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<float>::max_exponent - std::numeric_limits<float>::min_exponent + 1);
    }

    constexpr llvm::Value* isFiniteFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::getTrue(ctx);
    }

}

#endif
