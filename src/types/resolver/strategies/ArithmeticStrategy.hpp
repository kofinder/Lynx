/**
 * @file ArithmeticStrategy.hpp
 * @brief Defines the abstraction and implementations for arithmetic operations.
 *
 * This header declares the `ArithmeticStrategy` interface, providing methods
 * for basic arithmetic operations: addition, subtraction, multiplication,
 * division, and modulo. These operations are intended to generate LLVM IR
 * instructions corresponding to the operation on a given type.
 *
 * Key components:
 *  - `ArithmeticStrategy`: abstract base class defining arithmetic operations.
 *  - `ArithmeticStrategyImpl<T>`: template specialization for integer and
 *    floating-point types, enabling type-specific LLVM IR generation.
 *  - Type aliases (`ShortArithmeticStrategy`, `IntArithmeticStrategy`, etc.) 
 *    for convenience and clarity.
 *
 * Features:
 *  - Integer specialization demonstrates LLVM IR code generation for addition.
 *  - Floating-point specialization currently uses placeholders (`nullptr`).
 *  - Extensible design for implementing other arithmetic operations safely.
 *
 * Benefits:
 *  - Centralizes arithmetic logic for numeric types.
 *  - Provides a type-safe, uniform interface across different numeric types.
 *  - Facilitates LLVM IR code generation for compiler backends.
 *
 * Notes:
 *  - Only the integer `add` operation has a concrete implementation in this template.
 *  - Other operations and floating-point specializations are intended for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_ARITHMETIC_STRATEGY_HPP
#define LYNX_RESOLVER_ARITHMETIC_STRATEGY_HPP

#include <type_traits>
#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct ArithmeticStrategy {
        [[nodiscard]] virtual llvm::Value* add(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sub(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* mul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* div(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* mod(const StrategyContext&) const noexcept = 0;

        virtual ~ArithmeticStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct ArithmeticStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct ArithmeticStrategyImpl<T> : ArithmeticStrategy {
        [[nodiscard]] llvm::Value* add(const StrategyContext& stgCtx) const noexcept override { 
            std::cout << "WHAT THE FUCK ....." << std::endl;

            auto lhsPtr = stgCtx.instance;
            auto rhs = stgCtx.args[0];
            if (!lhsPtr || !rhs) return nullptr;

            auto& builder = stgCtx.ctx.getBuilder();

            auto* i8PtrTy = llvm::PointerType::get(lhsPtr->getContext(), 0);

            // Load the current value (type inferred from pointer)
            llvm::Value* lhsVal = builder.CreateLoad(i8PtrTy, lhsPtr, "load_lhs");

            // Cast rhs if needed
            if (lhsVal->getType() != rhs->getType()) {
                rhs = builder.CreateIntCast(rhs, lhsVal->getType(), true, "rhs_cast");
            }

            llvm::Value* result = builder.CreateAdd(lhsVal, rhs, "int_add");

            builder.CreateStore(result, lhsPtr);

            return result;
        }

        [[nodiscard]] llvm::Value* sub(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* mul(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* div(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* mod(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct ArithmeticStrategyImpl<T> : ArithmeticStrategy {
        [[nodiscard]] llvm::Value* add(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sub(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* mul(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* div(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* mod(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortArithmeticStrategy  = ArithmeticStrategyImpl<short>;
    using IntArithmeticStrategy    = ArithmeticStrategyImpl<int>;
    using LongArithmeticStrategy   = ArithmeticStrategyImpl<long>;
    using FloatArithmeticStrategy  = ArithmeticStrategyImpl<float>;
    using DoubleArithmeticStrategy = ArithmeticStrategyImpl<double>;
}

#endif
