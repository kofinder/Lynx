#ifndef LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP
#define LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct BitManipulationStrategy {
        [[nodiscard]] virtual llvm::Value* popCount(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* countLeadingZeros(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* countTrailingZeros(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* bitReverse(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* rotateLeft(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* rotateRight(const StrategyContext&) const noexcept = 0;

        virtual ~BitManipulationStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct BitManipulationStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct BitManipulationStrategyImpl<T> : BitManipulationStrategy {
        [[nodiscard]] llvm::Value* popCount(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* countLeadingZeros(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* countTrailingZeros(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitReverse(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* rotateLeft(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* rotateRight(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct BitManipulationStrategyImpl<T> : BitManipulationStrategy {
        [[nodiscard]] llvm::Value* popCount(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* countLeadingZeros(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* countTrailingZeros(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitReverse(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* rotateLeft(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* rotateRight(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortBitManipulationStrategy  = BitManipulationStrategyImpl<short>;
    using IntBitManipulationStrategy    = BitManipulationStrategyImpl<int>;
    using LongBitManipulationStrategy   = BitManipulationStrategyImpl<long>;
    using FloatBitManipulationStrategy  = BitManipulationStrategyImpl<float>;
    using DoubleBitManipulationStrategy = BitManipulationStrategyImpl<double>;
}

#endif 
