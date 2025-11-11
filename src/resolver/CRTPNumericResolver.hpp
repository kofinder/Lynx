#ifndef LYNX_CRTP_NUMERIC_RESOLVER_HPP
#define LYNX_CRTP_NUMERIC_RESOLVER_HPP

#include <type_traits>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <context/AstContext.hpp>
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/strategies/ArithmeticStrategy.hpp"
#include "resolver/strategies/BitwiseStrategy.hpp"
#include "resolver/strategies/AbsStrategy.hpp"
#include "resolver/strategies/MinMaxStrategy.hpp"
#include "resolver/strategies/ComparisonStrategy.hpp"
#include "resolver/strategies/MathStrategy.hpp"
#include "resolver/strategies/MemoryStrategy.hpp"
#include "resolver/strategies/BitManipulationStrategy.hpp"
#include "resolver/strategies/OverflowStrategy.hpp"
#include "resolver/strategies/SaturationStrategy.hpp"
#include "resolver/strategies/FixedPointStrategy.hpp"

namespace LynxResolver {

using namespace LynxContext;

// =========================
// Concepts for Numeric Types
// =========================
template<typename T>
concept Numeric = std::is_arithmetic_v<T>; // int, float, double, etc.

template<typename T>
concept Integer = std::is_integral_v<T>;

template<typename T>
concept Floating = std::is_floating_point_v<T>;

// =========================
// CRTP Base Resolver
// =========================
template<Numeric DerivedT, typename LLVMType>
class CRTPNumericResolver : public TypeMethodResolver {

    protected:

        const ArithmeticStrategy* arithmetic;
        const BitwiseStrategy* bitwise;
        const AbsStrategy* absStrat;
        const MinMaxStrategy* minMaxStrat;
        const ComparisonStrategy* cmpStrat;
        const MathStrategy* mathStrat;
        const MemoryStrategy* memStrat;
        const BitManipulationStrategy* bitManipStrat;
        const OverflowStrategy* overflowStrat;
        const SaturationStrategy* saturationStrat;
        const FixedPointStrategy* fixedPointStrat;

        // Derived class must provide this map
        static inline std::unordered_map<std::string, MethodName> MethodNameMap;

    public:
        // Constructor
        CRTPNumericResolver(
            const ArithmeticStrategy* arithmetic,
            const BitwiseStrategy* bitwise,
            const AbsStrategy* absStrat,
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
            const std::string& name,
            llvm::Value* instance,
            const std::vector<llvm::Value*>& args,
            std::shared_ptr<AstContext> ctx
        ) noexcept override {
            auto it = MethodNameMap.find(name);
            if (it == MethodNameMap.end()) return nullptr;

            MethodName method = it->second;

            if (!instance) {
                return DerivedT::resolveTypeMethod(method, args, ctx);
            }

            return DerivedT::resolveInstanceMethod(instance, args, method, ctx);
        }

        virtual ~CRTPNumericResolver() noexcept = default;
    };

}

#endif
