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
 * * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
 */


#ifndef LYNX_INT_METHOD_RESOLVER_HPP
#define LYNX_INT_METHOD_RESOLVER_HPP

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


    class IntMethodResolver {

        public:

            static llvm::Value* convertToString(llvm::Value* instance, std::shared_ptr<AstContext> ctx) noexcept;
            static llvm::Value* performClone(llvm::Value* instance, std::shared_ptr<AstContext> ctx) noexcept;
            static llvm::Value* performTypeCast(llvm::Value* instance, llvm::Type* targetType, std::shared_ptr<AstContext> ctx) noexcept;
            static llvm::Value* resolveTypeMethod(MethodName method, const std::vector<llvm::Value*>& args, std::shared_ptr<AstContext> ctx) noexcept;
            static llvm::Value* resolveInstanceMethod(llvm::Value* instance, const std::vector<llvm::Value*>& args, MethodName method, std::shared_ptr<AstContext> ctx) noexcept;

        public:

            using Base = TypeNumericResolver<IntMethodResolver, int>;
            using ToStr = ToStringMixin<IntMethodResolver>;
            using Cln = CloneMixin<IntMethodResolver>;
            using Cast = TypeCastMixin<IntMethodResolver>;

            struct Impl : public Base, public ToStr, public Cln, public Cast {
                IntArithmeticStrategy arithmetic;
                IntBitwiseStrategy bitwise;
                IntAbsStrategy absStrat;
                IntMinMaxStrategy minMaxStrat;
                IntComparisonStrategy cmpStrat;
                IntMathStrategy mathStrat;
                IntMemoryStrategy memStrat;
                IntBitManipulationStrategy bitManipStrat;
                IntOverflowStrategy overflowStrat;
                IntSaturationStrategy saturationStrat;
                IntFixedPointStrategy fixedPointStrat;
                Impl() noexcept : Base(
                    &arithmetic, 
                    &bitwise, 
                    &absStrat, 
                    &minMaxStrat, 
                    &cmpStrat, 
                    &mathStrat, 
                    &memStrat, 
                    &bitManipStrat, 
                    &overflowStrat, 
                    &saturationStrat, 
                    &fixedPointStrat
                ) {}
            };

            static std::unique_ptr<TypeMethodResolver> create() {
                return std::make_unique<Impl>();
            }         
    };
}

#endif


