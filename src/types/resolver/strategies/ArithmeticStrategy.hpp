/**
 * @file ArithmeticStrategy.hpp
 * @brief Defines the abstraction and implementations for absolute value and sign operations.
 *
 * This header declares the `ArithmeticStrategy` interface, which provides methods for
 * computing absolute value, negation, sign, clamping, and parity checks (`isEven`,
 * `isOdd`) for different numeric types in the Lynx type system.
 *
 * Key components:
 *  - `ArithmeticStrategy`: abstract base class defining the common operations.
 *  - `ArithmeticStrategyImpl<T>`: CRTP-style template for type-specific implementations.
 *    Specializations exist for integer and floating-point types.
 *  - Type aliases (`ShortArithmeticStrategy`, `IntArithmeticStrategy`, etc.) for convenience.
 *
 * Benefits:
 *  - Provides a uniform interface for numeric operations across multiple types.
 *  - Allows compile-time specialization via template concepts (`IntStrategyType`, `FloatStrategyType`).
 *  - Supports extension for additional numeric types or strategies.
 *
 * Notes:
 *  - Currently, the implementations return `nullptr` as placeholders.
 *  - Concrete LLVM IR generation should be provided in the respective specializations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_ABS_STRATEGY_HPP
#define LYNX_RESOLVER_ABS_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "llvm/Support/MathExtras.h"
#include "tmpl/TypeNumericPromotion.hpp"
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes {
    
    // ============================================================================
    // Base Interface
    // ============================================================================
    struct ArithmeticStrategy {
        [[nodiscard]] virtual llvm::Value* abs(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* negate(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sign(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* clamp(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isEven(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isOdd(const StrategyContext&) const noexcept = 0;
        virtual ~ArithmeticStrategy() noexcept = default;
    };

    // ============================================================================
    // Template forward declaration
    // ============================================================================
    template<typename T>
    struct ArithmeticStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct ArithmeticStrategyImpl<T> : ArithmeticStrategy {

        [[nodiscard]] llvm::Value* abs(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* zero = llvm::ConstantInt::get(type, 0);
            llvm::Value* isNeg = builder.CreateICmpSLT(val, zero);
            llvm::Value* negVal = builder.CreateNeg(val);
            llvm::Value* result = builder.CreateSelect(isNeg, negVal, val);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* negate(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto& builder = stg.ctx.getBuilder();
            llvm::Value* negVal = builder.CreateNeg(val);
            builder.CreateStore(negVal, stg.instancePtr);
            return negVal;
        }

        [[nodiscard]] llvm::Value* sign(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* zero = llvm::ConstantInt::get(type, 0);
            llvm::Value* one = llvm::ConstantInt::get(type, 1);
            llvm::Value* negOne = llvm::ConstantInt::get(type, -1);

            llvm::Value* isPos = builder.CreateICmpSGT(val, zero);
            llvm::Value* isNeg = builder.CreateICmpSLT(val, zero);
            llvm::Value* select = builder.CreateSelect(isPos, one, zero);
            llvm::Value* result = builder.CreateSelect(isNeg, negOne, select);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stg) const noexcept override {
            if (stg.args.size() < 2) return nullptr;
            llvm::Value* val = stg.instance;
            llvm::Value* minVal = stg.args[0];
            llvm::Value* maxVal = stg.args[1];
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* result = builder.CreateSelect(
                builder.CreateICmpSLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateICmpSGT(val, maxVal), maxVal, val)
            );

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* two = llvm::ConstantInt::get(type, 2);
            llvm::Value* zero = llvm::ConstantInt::get(type, 0);
            llvm::Value* rem = builder.CreateURem(val, two);
            llvm::Value* result = builder.CreateICmpEQ(rem, zero);

            return result;
        }

        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* two = llvm::ConstantInt::get(type, 2);
            llvm::Value* zero = llvm::ConstantInt::get(type, 0);
            llvm::Value* rem = builder.CreateURem(val, two);
            llvm::Value* result = builder.CreateICmpNE(rem, zero);

            return result;
        }
    };

    // ============================================================================
    // Floating-point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct ArithmeticStrategyImpl<T> : ArithmeticStrategy {

        [[nodiscard]] llvm::Value* abs(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* zero = llvm::ConstantFP::get(type, 0.0);
            llvm::Value* negVal = builder.CreateFNeg(val);
            llvm::Value* isNeg = builder.CreateFCmpOLT(val, zero);
            llvm::Value* result = builder.CreateSelect(isNeg, negVal, val);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* negate(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto& builder = stg.ctx.getBuilder();
            llvm::Value* result = builder.CreateFNeg(val);
            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* sign(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* zero = llvm::ConstantFP::get(type, 0.0);
            llvm::Value* one = llvm::ConstantFP::get(type, 1.0);
            llvm::Value* negOne = llvm::ConstantFP::get(type, -1.0);

            llvm::Value* isPos = builder.CreateFCmpOGT(val, zero);
            llvm::Value* isNeg = builder.CreateFCmpOLT(val, zero);
            llvm::Value* select = builder.CreateSelect(isPos, one, zero);
            llvm::Value* result = builder.CreateSelect(isNeg, negOne, select);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stg) const noexcept override {
            if (stg.args.size() < 2) return nullptr;
            llvm::Value* val = stg.instance;
            llvm::Value* minVal = stg.args[0];
            llvm::Value* maxVal = stg.args[1];
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* result = builder.CreateSelect(
                builder.CreateFCmpOLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateFCmpOGT(val, maxVal), maxVal, val)
            );

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* two = llvm::ConstantFP::get(type, 2.0);
            llvm::Value* zero = llvm::ConstantFP::get(type, 0.0);
            llvm::Value* rem = builder.CreateFRem(val, two);
            llvm::Value* result = builder.CreateFCmpUEQ(rem, zero);

            return result;
        }

        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            llvm::Type* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            llvm::Value* two = llvm::ConstantFP::get(type, 2.0);
            llvm::Value* zero = llvm::ConstantFP::get(type, 0.0);
            llvm::Value* rem = builder.CreateFRem(val, two);
            llvm::Value* result = builder.CreateFCmpUNE(rem, zero);

            return result;
        }
    };

    // ============================================================================
    // Type Aliases
    // ============================================================================
    using ShortArithmeticStrategy  = ArithmeticStrategyImpl<short>;
    using IntArithmeticStrategy    = ArithmeticStrategyImpl<int>;
    using LongArithmeticStrategy   = ArithmeticStrategyImpl<long>;
    using FloatArithmeticStrategy  = ArithmeticStrategyImpl<float>;
    using DoubleArithmeticStrategy = ArithmeticStrategyImpl<double>;
}

#endif 
