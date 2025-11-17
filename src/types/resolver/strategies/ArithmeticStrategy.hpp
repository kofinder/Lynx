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
#include "tmpl/TypeNumericPromotion.hpp"
#include "resolver/TypeStrategyContext.hpp"
#include "utils/TypeOperandsValidationUtils.hpp"
#include <constants/metadata/MetadataTypeConstants.hpp>

namespace LynxTypes {
    using namespace TypePromotion;
    using namespace MetadataTypeConstants;

    enum class ArithmeticOp { Add, Sub, Mul, Div, Mod };

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
    class ArithmeticStrategyImpl<T> : public ArithmeticStrategy {

        private:
        
            llvm::Value* generateBinaryOp(const StrategyContext& stgCtx, ArithmeticOp op) const noexcept {
                auto* lhs = stgCtx.instance;
                auto* lhsPtr = stgCtx.instancePtr;
                auto* rhs = stgCtx.args[0];
                auto& builder = stgCtx.ctx.getBuilder();

                if (!validateOperands(lhs, lhsPtr, rhs)) return nullptr;

                auto promoted = promoteNumericOperands(lhs, rhs, builder);
                promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
                promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);

                llvm::Value* result = nullptr;
                if (promoted.isFloating) {
                    switch (op) {
                        case ArithmeticOp::Add: result = builder.CreateFAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                        case ArithmeticOp::Sub: result = builder.CreateFSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                        case ArithmeticOp::Mul: result = builder.CreateFMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                        case ArithmeticOp::Div: result = builder.CreateFDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                        case ArithmeticOp::Mod: result = builder.CreateFRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
                    }
                } else {
                    switch (op) {
                        case ArithmeticOp::Add: result = builder.CreateAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                        case ArithmeticOp::Sub: result = builder.CreateSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                        case ArithmeticOp::Mul: result = builder.CreateMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                        case ArithmeticOp::Div: result = builder.CreateSDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                        case ArithmeticOp::Mod: result = builder.CreateSRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
                    }
                }

                builder.CreateStore(result, lhsPtr);
                return result;
            }

        public:

            [[nodiscard]] llvm::Value* add(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Add);}
            [[nodiscard]] llvm::Value* sub(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Sub); }
            [[nodiscard]] llvm::Value* mul(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Mul); }
            [[nodiscard]] llvm::Value* div(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Div); }
            [[nodiscard]] llvm::Value* mod(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Mod); }

    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    class ArithmeticStrategyImpl<T> : public ArithmeticStrategy {

        private:

            llvm::Value* generateBinaryOp(const StrategyContext& stgCtx, ArithmeticOp op) const noexcept {
                auto* lhs = stgCtx.instance;
                auto* lhsPtr = stgCtx.instancePtr;
                auto* rhs = stgCtx.args[0];
                auto& builder = stgCtx.ctx.getBuilder();

                if (!validateOperands(lhs, lhsPtr, rhs)) return nullptr;

                auto promoted = promoteNumericOperands(lhs, rhs, builder);
                promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
                promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);
        
                llvm::Value* result = nullptr;
                switch (op) {
                    case ArithmeticOp::Add: result = builder.CreateFAdd(promoted.lhs, promoted.rhs, OPR_ADD); break;
                    case ArithmeticOp::Sub: result = builder.CreateFSub(promoted.lhs, promoted.rhs, OPR_SUB); break;
                    case ArithmeticOp::Mul: result = builder.CreateFMul(promoted.lhs, promoted.rhs, OPR_MUL); break;
                    case ArithmeticOp::Div: result = builder.CreateFDiv(promoted.lhs, promoted.rhs, OPR_DIV); break;
                    case ArithmeticOp::Mod: result = builder.CreateFRem(promoted.lhs, promoted.rhs, OPR_MOD); break;
                }

                builder.CreateStore(result, lhsPtr);
                return result;
            }

        public:

            [[nodiscard]] llvm::Value* add(const StrategyContext& stgCtx) const noexcept override { 
                std::cout << "WOrking With floatiing point" << std::endl;
                return generateBinaryOp(stgCtx, ArithmeticOp::Add);
            }
            [[nodiscard]] llvm::Value* sub(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Sub); }
            [[nodiscard]] llvm::Value* mul(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Mul); }
            [[nodiscard]] llvm::Value* div(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Div); }
            [[nodiscard]] llvm::Value* mod(const StrategyContext& stgCtx) const noexcept override { return generateBinaryOp(stgCtx, ArithmeticOp::Mod); }
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
