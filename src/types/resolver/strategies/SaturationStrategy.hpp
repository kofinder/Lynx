/**
 * @file SaturationStrategy.hpp
 * @brief Defines the SaturationStrategy interface and template specializations for numeric types.
 *
 * This header provides an abstraction for performing arithmetic operations
 * with saturation behavior, where results are clamped to the type's numeric
 * limits instead of overflowing. It allows LLVM IR code generation for
 * signed and unsigned arithmetic with saturation semantics.
 *
 * Key components:
 *  - `SaturationStrategy`: abstract base class defining signed/unsigned arithmetic with saturation methods.
 *  - `SaturationStrategyImpl<T>`: template specializations for integer and floating-point types.
 *  - Type aliases (`ShortSaturationStrategy`, `IntSaturationStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Default implementations return `nullptr` as placeholders.
 *  - Designed to integrate with LLVM IR backend for safe numeric operations.
 *
 * Benefits:
 *  - Centralizes saturation-aware arithmetic logic for numeric types.
 *  - Provides a uniform interface for compiler code generation.
 *  - Improves safety and maintainability in numeric computations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_SATURATION_STRATEGY_HPP
#define LYNX_RESOLVER_SATURATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "resolver/TypeStrategyContext.hpp"
#include "helpers/InstructionHelper.hpp"

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

        // ---------------------------
        // Signed Add Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* saddSat(const StrategyContext& stg) const noexcept override { 
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* a = stg.args[0];
            llvm::Value* b = stg.args[1];
            llvm::Type* type = a->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::sadd_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {a, b});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;    
        }

        // ---------------------------
        // Unsigned Add Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* uaddSat(const StrategyContext& stg) const noexcept override { 
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* a = stg.args[0];
            llvm::Value* b = stg.args[1];
            llvm::Type* type = a->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::uadd_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {a, b});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;    
        }

        // ---------------------------
        // Signed Sub Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* ssubSat(const StrategyContext& stg) const noexcept override {             
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* a = stg.args[0];
            llvm::Value* b = stg.args[1];
            llvm::Type* type = a->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::ssub_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {a, b});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        // ---------------------------
        // Unsigned Sub Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* usubSat(const StrategyContext& stg) const noexcept override {
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* a = stg.args[0];
            llvm::Value* b = stg.args[1];
            llvm::Type* type = a->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::usub_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {a, b});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;    
        }

        // ---------------------------
        // Signed Shift-Left Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* sshlSat(const StrategyContext& stg) const noexcept override { 
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* value = stg.args[0];
            llvm::Value* shift = stg.args[1];
            llvm::Type* type = value->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::sshl_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {value, shift});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;
        }

        // ---------------------------
        // Unsigned Shift-Left Saturating
        // ---------------------------
        [[nodiscard]] llvm::Value* ushLSat(const StrategyContext& stg) const noexcept override { 
            if (stg.args.size() < 2) return nullptr;

            llvm::Value* value = stg.args[0];
            llvm::Value* shift = stg.args[1];
            llvm::Type* type = value->getType();

            auto& builder = stg.ctx.getBuilder();
            auto* module = stg.ctx.getModule();
            llvm::Function* fn = helper::callOfIntrinsic(module, llvm::Intrinsic::ushl_sat, type);
            llvm::Value* result = builder.CreateCall(fn, {value, shift});
            if (stg.instancePtr) builder.CreateStore(result, stg.instancePtr);
            return result;
        }
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
