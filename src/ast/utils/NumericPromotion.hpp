#ifndef LYNX_NUMERIC_PROMOTION_UTILS_HPP
#define LYNX_NUMERIC_PROMOTION_UTILS_HPP

#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <types/tmpl/TypeChecker.hpp>

namespace LynxAst::TypePromotion {

    using namespace LynxTypes;

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
        if constexpr (std::same_as<T, ByteType>)    return 1;
        else if constexpr (std::same_as<T, ShortType>)   return 2;
        else if constexpr (std::same_as<T, CharType>)    return 3;
        else if constexpr (std::same_as<T, IntegerType>) return 4;
        else if constexpr (std::same_as<T, LongType>)    return 5;
        else if constexpr (std::same_as<T, FloatType>)   return 6;
        else if constexpr (std::same_as<T, DoubleType>)  return 7;
        else return 0;
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
        PromotedNumeric result{ lhs, rhs, nullptr, false };
        if (!lhs || !rhs) return result;

        llvm::Type* lhsType = lhs->getType();
        llvm::Type* rhsType = rhs->getType();

        int lhsRank = getNumericRank(lhsType);
        int rhsRank = getNumericRank(rhsType);

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
    inline llvm::Value* matchConstantType(llvm::IRBuilder<>& builder, llvm::Value* value, llvm::Type* targetType) noexcept {
        if (llvm::isa<llvm::ConstantInt>(value) && targetType->isFloatingPointTy()) {
            auto ci = llvm::cast<llvm::ConstantInt>(value);
            return llvm::ConstantFP::get(targetType, ci->getSExtValue());
        }
        return value;
    }

}
#endif
