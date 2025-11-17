/**
 * @file FloatMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `float` type.
 *
 * `FloatMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `float` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * FloatMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("sqrt", floatInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `float` type.
 *
 * * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
*/


#ifndef LYNX_FLOAT_METHOD_RESOLVER_HPP
#define LYNX_FLOAT_METHOD_RESOLVER_HPP


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

    class FloatMethodResolver : public TypeMethodResolver {

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

            using Base = TypeNumericResolver<FloatMethodResolver, float>;
            using ToStr = ToStringMixin<FloatMethodResolver>;
            using Cln = CloneMixin<FloatMethodResolver>;
            using Cast = TypeCastMixin<FloatMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                FloatArithmeticStrategy arithmetic;
                FloatBitwiseStrategy bitwise;
                FloatAbsStrategy abs;
                FloatMinMaxStrategy minMax;
                FloatComparisonStrategy cmp;
                FloatMathStrategy math;
                FloatMemoryStrategy mem;
                FloatBitManipulationStrategy bitManip;
                FloatOverflowStrategy overflow;
                FloatSaturationStrategy saturation;
                FloatFixedPointStrategy fixedPoint;
                Impl() noexcept : Base(&arithmetic, &bitwise, &abs, &minMax, &cmp, &math, &mem, &bitManip, &overflow, &saturation, &fixedPoint) {}
            };

            static TypeMethodResolver* create() { return new Impl(); } 
    };
}

#endif
