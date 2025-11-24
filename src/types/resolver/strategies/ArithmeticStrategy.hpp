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
 * @author: Ko Thein (Nathan Mratt)
 * @date:   Jan 2, 2022
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
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes {

    using namespace LynxConstants;

    
    // ============================================================================
    // Base Interface
    // ============================================================================
    struct ArithmeticStrategy {

        ArithmeticStrategy() = default;
        
        // Rule of Five
        ArithmeticStrategy(const ArithmeticStrategy&) = delete;
        ArithmeticStrategy& operator=(const ArithmeticStrategy&) = delete;
        ArithmeticStrategy(ArithmeticStrategy&&) = delete;
        ArithmeticStrategy& operator=(ArithmeticStrategy&&) = delete;
        virtual ~ArithmeticStrategy() noexcept = default;    

        [[nodiscard]] virtual llvm::Value* abs(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* negate(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sign(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* clamp(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isEven(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isOdd(const StrategyContext&) const noexcept = 0;
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
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* isNeg = builder.CreateICmpSLT(val, zero);
            auto* negVal = builder.CreateNeg(val);
            auto* result = builder.CreateSelect(isNeg, negVal, val);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* negate(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto& builder = stg.ctx.getBuilder();
            auto* negVal = builder.CreateNeg(val);
            builder.CreateStore(negVal, stg.instancePtr);
            return negVal;
        }

        [[nodiscard]] llvm::Value* sign(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* one = llvm::ConstantInt::get(type, 1);
            auto* negOne = llvm::ConstantInt::get(type, -1);

            auto* isPos = builder.CreateICmpSGT(val, zero);
            auto* isNeg = builder.CreateICmpSLT(val, zero);
            auto* select = builder.CreateSelect(isPos, one, zero);
            auto* result = builder.CreateSelect(isNeg, negOne, select);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stg) const noexcept override {
            if (stg.args.size() < 2) return nullptr;
            llvm::Value* val = stg.instance;
            auto* minVal = stg.args[0];
            auto* maxVal = stg.args[1];
            auto& builder = stg.ctx.getBuilder();

            auto* result = builder.CreateSelect(
                builder.CreateICmpSLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateICmpSGT(val, maxVal), maxVal, val)
            );

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* two = llvm::ConstantInt::get(type, 2);
            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* rem = builder.CreateURem(val, two);
            auto* result = builder.CreateICmpEQ(rem, zero);

            return result;
        }

        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* two = llvm::ConstantInt::get(type, 2);
            auto* zero = llvm::ConstantInt::get(type, 0);
            auto* rem = builder.CreateURem(val, two);
            auto* result = builder.CreateICmpNE(rem, zero);

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
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* negVal = builder.CreateFNeg(val);
            auto* isNeg = builder.CreateFCmpOLT(val, zero);
            auto* result = builder.CreateSelect(isNeg, negVal, val);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* negate(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto& builder = stg.ctx.getBuilder();
            auto* result = builder.CreateFNeg(val);
            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* sign(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* one = llvm::ConstantFP::get(type, 1.0);
            auto* negOne = llvm::ConstantFP::get(type, -1.0);

            auto* isPos = builder.CreateFCmpOGT(val, zero);
            auto* isNeg = builder.CreateFCmpOLT(val, zero);
            auto* select = builder.CreateSelect(isPos, one, zero);
            auto* result = builder.CreateSelect(isNeg, negOne, select);

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stg) const noexcept override {
            if (stg.args.size() < 2) return nullptr;
            llvm::Value* val = stg.instance;
            auto* minVal = stg.args[0];
            auto* maxVal = stg.args[1];
            auto& builder = stg.ctx.getBuilder();

            auto* result = builder.CreateSelect(
                builder.CreateFCmpOLT(val, minVal), minVal,
                builder.CreateSelect(builder.CreateFCmpOGT(val, maxVal), maxVal, val)
            );

            builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* two = llvm::ConstantFP::get(type, FP_TWO);
            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* rem = builder.CreateFRem(val, two);
            auto* result = builder.CreateFCmpUEQ(rem, zero);

            return result;
        }

        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stg) const noexcept override {
            llvm::Value* val = stg.instance;
            auto* type = val->getType();
            auto& builder = stg.ctx.getBuilder();

            auto* two = llvm::ConstantFP::get(type, FP_TWO);
            auto* zero = llvm::ConstantFP::get(type, 0.0);
            auto* rem = builder.CreateFRem(val, two);
            auto* result = builder.CreateFCmpUNE(rem, zero);

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
