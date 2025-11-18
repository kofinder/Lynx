/**
 * @file TypeNumericResolver.hpp
 * @brief Provides numeric type resolution logic for Lynx using strategy-based operations.
 *
 * The `TypeNumericResolver` class template serves as a CRTP-based resolver for numeric types in
 * the Lynx type system. It integrates multiple operation strategies—such as arithmetic, bitwise,
 * comparison, overflow handling, and fixed-point logic—to support method resolution on both
 * built-in and user-defined numeric types. This design allows extensible and modular behavior
 * while keeping type-specific logic isolated.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_NUMERIC_RESOLVER_HPP
#define LYNX_TYPE_NUMERIC_RESOLVER_HPP

#include <type_traits>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <context/AstContext.hpp>
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/strategies/BinaryStrategy.hpp"
#include "resolver/strategies/BitwiseStrategy.hpp"
#include "resolver/strategies/ArithmeticStrategy.hpp"
#include "resolver/strategies/MinMaxStrategy.hpp"
#include "resolver/strategies/ComparisonStrategy.hpp"
#include "resolver/strategies/MathStrategy.hpp"
#include "resolver/strategies/MemoryStrategy.hpp"
#include "resolver/strategies/BitManipulationStrategy.hpp"
#include "resolver/strategies/OverflowStrategy.hpp"
#include "resolver/strategies/SaturationStrategy.hpp"
#include "resolver/strategies/FixedPointStrategy.hpp"

namespace LynxTypes {

    // =========================
    // CRTP Base Resolver
    // =========================
    template<typename DerivedT, typename LLVMType>
    class TypeNumericResolver : public TypeMethodResolver {

        protected:

            const BinaryStrategy* arithmetic;
            const BitwiseStrategy* bitwise;
            const ArithmeticStrategy* absStrat;
            const MinMaxStrategy* minMaxStrat;
            const ComparisonStrategy* cmpStrat;
            const MathStrategy* mathStrat;
            const MemoryStrategy* memStrat;
            const BitManipulationStrategy* bitManipStrat;
            const OverflowStrategy* overflowStrat;
            const SaturationStrategy* saturationStrat;
            const FixedPointStrategy* fixedPointStrat;

        public:
            
            TypeNumericResolver(
                const BinaryStrategy* arithmetic,
                const BitwiseStrategy* bitwise,
                const ArithmeticStrategy* absStrat,
                const MinMaxStrategy* minMaxStrat,
                const ComparisonStrategy* cmpStrat,
                const MathStrategy* mathStrat,
                const MemoryStrategy* memStrat,
                const BitManipulationStrategy* bitManipStrat,
                const OverflowStrategy* overflowStrat,
                const SaturationStrategy* saturationStrat,
                const FixedPointStrategy* fixedPointStrat
            ) noexcept
                : arithmetic(arithmetic)
                , bitwise(bitwise)
                , absStrat(absStrat)
                , minMaxStrat(minMaxStrat)
                , cmpStrat(cmpStrat)
                , mathStrat(mathStrat)
                , memStrat(memStrat)
                , bitManipStrat(bitManipStrat)
                , overflowStrat(overflowStrat)
                , saturationStrat(saturationStrat)
                , fixedPointStrat(fixedPointStrat)
            {}

            // Resolve method (CRTP + TypeMethodResolver)
            [[nodiscard]] llvm::Value* resolveMethod(
                const AstContext& ctx,
                llvm::Value* instance,
                llvm::Value* instancePtr,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept override {

                if (!instance) return DerivedT::resolveTypeMethod(ctx, method, args);

                return DerivedT::resolveInstanceMethod(ctx, instance, instancePtr, method, args);
            }

            virtual ~TypeNumericResolver() noexcept = default;
    };

}

#endif
