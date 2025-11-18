/**
 * @file LongMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `long` type.
 *
 * `LongMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `long` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * LongMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toString", longInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `long` type.
*/


#ifndef LYNX_LONG_METHOD_RESOLVER_HPP
#define LYNX_LONG_METHOD_RESOLVER_HPP

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

    class LongMethodResolver {

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

            using Base = TypeNumericResolver<LongMethodResolver, long>;
            using ToStr = ToStringMixin<LongMethodResolver>;
            using Cln = CloneMixin<LongMethodResolver>;
            using Cast = TypeCastMixin<LongMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                LongBinaryStrategy arithmetic;
                LongBitwiseStrategy bitwise;
                LongArithmeticStrategy abs;
                LongMinMaxStrategy minMax;
                LongComparisonStrategy cmp;
                LongMathStrategy math;
                LongMemoryStrategy mem;
                LongBitManipulationStrategy bitManip;
                LongOverflowStrategy overflow;
                LongSaturationStrategy saturation;
                LongFixedPointStrategy fixedPoint;
                Impl() noexcept : Base(&arithmetic, &bitwise, &abs, &minMax, &cmp, &math, &mem, &bitManip, &overflow, &saturation, &fixedPoint) {}
            };

            static TypeMethodResolver* create() { return new Impl(); }         
    };
}

#endif
