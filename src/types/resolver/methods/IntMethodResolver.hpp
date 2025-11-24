/**
 * @file IntMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `int` type.
 *
 * `IntMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `int` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * IntMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("abs", intInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `int` type.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */


#ifndef LYNX_INT_METHOD_RESOLVER_HPP
#define LYNX_INT_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"
#include "resolver/TypeNumericResolver.hpp"
#include "resolver/TypeResolverMixins.hpp"

#include "strategies/BitwiseStrategy.hpp"
#include "strategies/ArithmeticStrategy.hpp"
#include "strategies/MinMaxStrategy.hpp"
#include "strategies/ComparisonStrategy.hpp"
#include "strategies/MathStrategy.hpp"
#include "strategies/MemoryStrategy.hpp"
#include "strategies/BitManipulationStrategy.hpp"
#include "strategies/OverflowStrategy.hpp"
#include "strategies/SaturationStrategy.hpp"
#include "strategies/FixedPointStrategy.hpp"

namespace LynxTypes {

    class IntMethodResolver {

        public:

            static llvm::Value* convertToString(const AstContext& ctx, llvm::Value* instance) noexcept;

            static llvm::Value* performClone(const AstContext& ctx, llvm::Value* instance) noexcept;

            static llvm::Value* performTypeCast(const AstContext& ctx, llvm::Value* instance, llvm::Type* targetType) noexcept;

            static llvm::Value* resolveTypeMethod(const AstContext& ctx, const std::string& method, const std::vector<llvm::Value*>& args) noexcept;
            
            static llvm::Value* resolveInstanceMethod(
                const AstContext& ctx,
                llvm::Value* instance,
                llvm::Value* instancePtr,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept;

        public:

            using Base = TypeNumericResolver<IntMethodResolver, int>;
            using ToStr = ToStringMixin<IntMethodResolver>;
            using Cln = CloneMixin<IntMethodResolver>;
            using Cast = TypeCastMixin<IntMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                IntBinaryStrategy arithmetic;
                IntBitwiseStrategy bitwise;
                IntArithmeticStrategy abs;
                IntMinMaxStrategy minMax;
                IntComparisonStrategy cmp;
                IntMathStrategy math;
                IntMemoryStrategy mem;
                IntBitManipulationStrategy bitManip;
                IntOverflowStrategy overflow;
                IntSaturationStrategy saturation;
                IntFixedPointStrategy fixedPoint;
                Impl() noexcept : Base(&arithmetic, &bitwise, &abs, &minMax, &cmp, &math, &mem, &bitManip, &overflow, &saturation, &fixedPoint) {}
            };

            static TypeMethodResolver* create() { 
                return new Impl(); // NOLINT(cppcoreguidelines-owning-memory) 
            }         
    };
}

#endif


