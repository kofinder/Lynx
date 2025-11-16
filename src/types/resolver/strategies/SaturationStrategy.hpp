#ifndef LYNX_RESOLVER_SATURATION_STRATEGY_HPP
#define LYNX_RESOLVER_SATURATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct SaturationStrategy {
        [[nodiscard]] virtual llvm::Value* saddSat(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* uaddSat(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ssubSat(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* usubSat(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sshlSat(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ushLSat(const StrategyContext&) const noexcept = 0;

        virtual ~SaturationStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct SaturationStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct SaturationStrategyImpl<T> : SaturationStrategy {
        [[nodiscard]] llvm::Value* saddSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* uaddSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ssubSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* usubSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sshlSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ushLSat(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct SaturationStrategyImpl<T> : SaturationStrategy {
        [[nodiscard]] llvm::Value* saddSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* uaddSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ssubSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* usubSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sshlSat(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ushLSat(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortSaturationStrategy  = SaturationStrategyImpl<short>;
    using IntSaturationStrategy    = SaturationStrategyImpl<int>;
    using LongSaturationStrategy   = SaturationStrategyImpl<long>;
    using FloatSaturationStrategy  = SaturationStrategyImpl<float>;
    using DoubleSaturationStrategy = SaturationStrategyImpl<double>;
}

#endif 
