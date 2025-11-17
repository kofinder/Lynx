/**
 * @file ShortMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `short` type.
 *
 * `ShortMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `short` instances in Lynx. It maps method names to their
 * corresponding LLVM IR implementations.
 *
 * Example usage:
 * @code
 * ShortMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toString", shortInstance, {}, astContext);
 * @endcode
 *
 * Methods like arithmetic operations, comparisons, and type conversions
 * are resolved according to the semantics of the Lynx `short` type.
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_SHORT_METHOD_RESOLVER_HPP
#define LYNX_SHORT_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"
#include "resolver/TypeNumericResolver.hpp"
#include "resolver/TypeResolverMixins.hpp"

#include "strategies/BitwiseStrategy.hpp"
#include "strategies/AbsStrategy.hpp"
#include "strategies/MinMaxStrategy.hpp"
#include "strategies/ComparisonStrategy.hpp"
#include "strategies/MathStrategy.hpp"
#include "strategies/MemoryStrategy.hpp"
#include "strategies/BitManipulationStrategy.hpp"
#include "strategies/OverflowStrategy.hpp"
#include "strategies/SaturationStrategy.hpp"
#include "strategies/FixedPointStrategy.hpp"

namespace LynxTypes {

    class ShortMethodResolver {

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

            using Base = TypeNumericResolver<ShortMethodResolver, short>;
            using ToStr = ToStringMixin<ShortMethodResolver>;
            using Cln = CloneMixin<ShortMethodResolver>;
            using Cast = TypeCastMixin<ShortMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                ShortArithmeticStrategy arithmetic;
                ShortBitwiseStrategy bitwise;
                ShortAbsStrategy abs;
                ShortMinMaxStrategy minMax;
                ShortComparisonStrategy cmp;
                ShortMathStrategy math;
                ShortMemoryStrategy mem;
                ShortBitManipulationStrategy bitManip;
                ShortOverflowStrategy overflow;
                ShortSaturationStrategy saturation;
                ShortFixedPointStrategy fixedPoint;
                Impl() noexcept : Base(&arithmetic, &bitwise, &abs, &minMax, &cmp, &math, &mem, &bitManip, &overflow, &saturation, &fixedPoint) {}
            };

            static TypeMethodResolver* create() { return new Impl(); }         
    };
}

#endif