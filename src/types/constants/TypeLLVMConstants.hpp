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
 * These functions are implemented as `inline inline` to allow
 * constant-folding where possible and avoid ODR violations across translation units.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_TYPE_LLVM_CONSTANTS_HPP
#define LYNX_TYPE_LLVM_CONSTANTS_HPP

#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/ADT/APFloat.h>
#include <limits>

namespace LynxTypes {

    inline unsigned SHORT_BIT_WIDTH  = 16;
    inline unsigned INT_BIT_WIDTH   = 32;
    inline unsigned LONG_BIT_WIDTH  = 64;



    // ======================================================
    // INTEGER CONSTANTS
    // ======================================================
    inline llvm::Value* intMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::max(), true); 
    }

    inline llvm::Value* intMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<int>::min(), true); 
    }

    inline llvm::Value* shortMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::max(), true); 
    }

    inline llvm::Value* shortMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::numeric_limits<short>::min(), true); 
    }

    inline llvm::Value* longMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::max(), true); 
    }

    inline llvm::Value* longMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::numeric_limits<long>::min(), true); 
    }

    inline llvm::Value* charMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::max(), true); 
    }

    inline llvm::Value* charMin(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::numeric_limits<char>::min(), true); 
    }

    inline llvm::Value* uintMax(llvm::LLVMContext& ctx)  noexcept { 
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<unsigned int>::max(), false); 
    }

    // ======================================================
    // FLOATING-POINT CONSTANTS
    // ======================================================
    inline llvm::Value* floatMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::max());
    }

    inline llvm::Value* floatMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::lowest());
    }

    inline llvm::Value* doubleMax(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::max());
    }

    inline llvm::Value* doubleMin(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::lowest());
    }

    inline llvm::Value* floatEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::epsilon());
    }

    inline llvm::Value* doubleEpsilon(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::numeric_limits<double>::epsilon());
    }

    // ======================================================
    // SPECIAL FLOATING-POINT VALUES
    // ======================================================
    inline llvm::Value* floatPosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), false));
    }

    inline llvm::Value* floatNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEsingle(), true));
    }

    inline llvm::Value* doublePosInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), false));
    }

    inline llvm::Value* doubleNegInfinity(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getInf(llvm::APFloat::IEEEdouble(), true));
    }

    inline llvm::Value* floatNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEsingle()));
    }

    inline llvm::Value* doubleNaN(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble()));
    }

    inline llvm::Value* floatZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0F));
    }

    inline llvm::Value* floatNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0F));
    }

    inline llvm::Value* doubleZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(0.0));
    }

    inline llvm::Value* doubleNegZero(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(ctx, llvm::APFloat(-0.0));
    }

    // ======================================================
    // GENERIC CONSTANTS (dynamic by type)
    // ======================================================
    inline llvm::Value* zero(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 0.0)
                                         : llvm::ConstantInt::get(type, 0);
    }

    inline llvm::Value* one(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
        return type->isFloatingPointTy() ? llvm::ConstantFP::get(type, 1.0)
                                         : llvm::ConstantInt::get(type, 1);
    }

    inline llvm::Value* negativeOne(llvm::LLVMContext& /*unused*/, llvm::Type* type) {
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
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), INT_BIT_WIDTH);
        if (type->isDoubleTy()) 
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), LONG_BIT_WIDTH);
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
    inline llvm::Value* allOnesInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ~0U);
    }

    inline llvm::Value* halfMaxInt32(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<int32_t>::max() / 2);
    }

    inline llvm::Value* minPositiveFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::numeric_limits<float>::min());
    }

    inline llvm::Value* mantissaBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::numeric_limits<float>::digits);
    }

    inline llvm::Value* exponentBitsFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
            std::numeric_limits<float>::max_exponent - std::numeric_limits<float>::min_exponent + 1);
    }

    inline llvm::Value* isFiniteFloat(llvm::LLVMContext& ctx)  noexcept {
        return llvm::ConstantInt::getTrue(ctx);
    }

}

#endif
