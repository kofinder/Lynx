/**
 * @file DoubleMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `double` type.
 *
 * `DoubleMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `double` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * DoubleMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("sqrt", doubleInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `double` type.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_DOUBLE_METHOD_RESOLVER_HPP
#define LYNX_DOUBLE_METHOD_RESOLVER_HPP

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

    class DoubleMethodResolver : public TypeMethodResolver {

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

            using Base = TypeNumericResolver<DoubleMethodResolver, double>;
            using ToStr = ToStringMixin<DoubleMethodResolver>;
            using Cln = CloneMixin<DoubleMethodResolver>;
            using Cast = TypeCastMixin<DoubleMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                DoubleArithmeticStrategy arithmetic;
                DoubleBitwiseStrategy bitwise;
                DoubleAbsStrategy abs;
                DoubleMinMaxStrategy minMax;
                DoubleComparisonStrategy cmp;
                DoubleMathStrategy math;
                DoubleMemoryStrategy mem;
                DoubleBitManipulationStrategy bitManip;
                DoubleOverflowStrategy overflow;
                DoubleSaturationStrategy saturation;
                DoubleFixedPointStrategy fixedPoint;
                Impl() noexcept : Base(&arithmetic, &bitwise, &abs, &minMax, &cmp, &math, &mem, &bitManip, &overflow, &saturation, &fixedPoint) {}
            };

            static TypeMethodResolver* create() { return new Impl(); } 
    };
}

#endif