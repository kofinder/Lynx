/**
 * @file NumericPromotionUtils.hpp
 * @brief Utilities for promoting numeric LLVM values to a common type for binary operations.
 *
 * Provides compile-time and runtime numeric type ranking, type checking,
 * and promotion of operands to a common type using LLVM IRBuilder.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 4, 2025
*/


#ifndef LYNX_TYPE_NUMERIC_PROMOTION_HPP
#define LYNX_TYPE_NUMERIC_PROMOTION_HPP

#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include "TypeChecker.hpp"
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes::TypePromotion {

    using namespace LynxConstants;

    struct PromotedNumeric {
        llvm::Value* lhs{nullptr};
        llvm::Value* rhs{nullptr};
        llvm::Type* commonType{nullptr};
        bool isFloating{false};
    };

    // ==========================
    // Concept for numeric types
    // ==========================
    template<typename T>
    concept NumericType = std::same_as<T, ByteType>   || std::same_as<T, ShortType> ||
                          std::same_as<T, CharType>   || std::same_as<T, IntegerType> ||
                          std::same_as<T, LongType>   || std::same_as<T, FloatType> ||
                          std::same_as<T, DoubleType>;

    // ==========================
    // Numeric rank constexpr
    // ==========================
    template<NumericType T>
    constexpr int typeRank() noexcept {
        if constexpr (std::same_as<T, ByteType>)         return RANK_ONE;
        else if constexpr (std::same_as<T, ShortType>)   return RANK_TWO;
        else if constexpr (std::same_as<T, CharType>)    return RANK_THREE;
        else if constexpr (std::same_as<T, IntegerType>) return RANK_FOUR;
        else if constexpr (std::same_as<T, LongType>)    return RANK_FIVE;
        else if constexpr (std::same_as<T, FloatType>)   return RANK_SIX;
        else if constexpr (std::same_as<T, DoubleType>)  return RANK_SEVEN;
        else return RANK_ZERO;
    }

    // ==========================
    // Runtime numeric rank
    // ==========================
    inline int getNumericRank(llvm::Type* type) noexcept {
        if (TypeChecker::is<ByteType>(type))    return typeRank<ByteType>();
        if (TypeChecker::is<ShortType>(type))   return typeRank<ShortType>();
        if (TypeChecker::is<CharType>(type))    return typeRank<CharType>();
        if (TypeChecker::is<IntegerType>(type)) return typeRank<IntegerType>();
        if (TypeChecker::is<LongType>(type))    return typeRank<LongType>();
        if (TypeChecker::is<FloatType>(type))   return typeRank<FloatType>();
        if (TypeChecker::is<DoubleType>(type))  return typeRank<DoubleType>();
        return 0;
    }

    // ==========================
    // Check if LLVM type is numeric
    // ==========================
    inline bool isNumericType(llvm::Type* type) noexcept {
        return getNumericRank(type) > 0;
    }

    // ==========================
    // Promote operands to common type
    // ==========================
    inline PromotedNumeric promoteNumericOperands(llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>& builder) noexcept {
        PromotedNumeric result{
            .lhs = lhs,
            .rhs = rhs,
            .commonType = nullptr,
            .isFloating = false
        };

        if (!lhs || !rhs) return result;

        llvm::Type* lhsType = lhs->getType();
        llvm::Type* rhsType = rhs->getType();

        const int lhsRank = getNumericRank(lhsType);
        const int rhsRank = getNumericRank(rhsType);

        llvm::Type* targetType = (lhsRank >= rhsRank) ? lhsType : rhsType;

        auto castValue = [&](llvm::Value* val, llvm::Type* toType) -> llvm::Value* {
            if (val->getType()->isIntegerTy() && toType->isFloatingPointTy())
                return builder.CreateSIToFP(val, toType, "cast_int_to_fp");
            if (val->getType()->isFloatingPointTy() && val->getType() != toType)
                return builder.CreateFPExt(val, toType, "cast_fp");
            if (val->getType()->isIntegerTy() && toType->isIntegerTy() && val->getType() != toType)
                return builder.CreateIntCast(val, toType, true, "cast_int");
            return val;
        };

        result.lhs = castValue(lhs, targetType);
        result.rhs = castValue(rhs, targetType);
        result.commonType = targetType;
        result.isFloating = targetType->isFloatingPointTy();        

        return result;
    }

    // ==========================
    // Match constant value type
    // ==========================
    inline llvm::Value* matchConstantType(llvm::IRBuilder<>& /*builder*/, llvm::Value* value, llvm::Type* targetType) noexcept {
        if (llvm::isa<llvm::ConstantInt>(value) && targetType->isFloatingPointTy()) {
            auto* cii = llvm::cast<llvm::ConstantInt>(value);
            const llvm::APFloat apf(static_cast<double>(cii->getSExtValue()));
            return llvm::ConstantFP::get(targetType, apf);
        }
        return value;
    }

}
#endif
