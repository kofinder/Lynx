/**
 * @file MemoryStrategy.hpp
 * @brief Defines the MemoryStrategy interface and template specializations for numeric types.
 *
 * This header provides an abstraction for memory-related operations such as
 * `memcpy`, `memmove`, `memset`, and `memsetPattern`, enabling LLVM IR code
 * generation for memory manipulation across different numeric types in the Lynx type system.
 *
 * Key components:
 *  - `MemoryStrategy`: abstract base class defining memory operation methods.
 *  - `MemoryStrategyImpl<T>`: template specializations for integer and floating-point types.
 *  - Type aliases (`ShortMemoryStrategy`, `IntMemoryStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Default implementations return `nullptr` as placeholders.
 *  - Designed to integrate with backend LLVM IR generation for compiler operations.
 *
 * Benefits:
 *  - Centralizes memory operation logic for numeric types.
 *  - Ensures a uniform interface for compiler code generation.
 *  - Improves maintainability and consistency in low-level memory handling.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/


#ifndef LYNX_RESOLVER_MEMORY_STRATEGY_HPP
#define LYNX_RESOLVER_MEMORY_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct MemoryStrategy {

        MemoryStrategy() = default;
    
        // Rule of Fives
        MemoryStrategy(const MemoryStrategy&) = delete;
        MemoryStrategy& operator=(const MemoryStrategy&) = delete;
        MemoryStrategy(MemoryStrategy&&) = delete;
        MemoryStrategy& operator=(MemoryStrategy&&) = delete;
        virtual ~MemoryStrategy() noexcept = default; 
            
        [[nodiscard]] virtual llvm::Value* memcpy(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memmove(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memset(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memsetPattern(const StrategyContext&) const noexcept = 0;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct MemoryStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct MemoryStrategyImpl<T> : MemoryStrategy {
        [[nodiscard]] llvm::Value* memcpy(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memmove(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memset(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memsetPattern(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MemoryStrategyImpl<T> : MemoryStrategy {
        [[nodiscard]] llvm::Value* memcpy(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memmove(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memset(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memsetPattern(const StrategyContext& /*unused*/) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortMemoryStrategy  = MemoryStrategyImpl<short>;
    using IntMemoryStrategy    = MemoryStrategyImpl<int>;
    using LongMemoryStrategy   = MemoryStrategyImpl<long>;
    using FloatMemoryStrategy  = MemoryStrategyImpl<float>;
    using DoubleMemoryStrategy = MemoryStrategyImpl<double>;
}

#endif 
