#include "methods/IntMethodResolver.hpp"
#include "resolver/TypeNumericConstants.hpp"

namespace LynxResolver {

    llvm::Value* IntMethodResolver::convertToString(llvm::Value* instance, std::shared_ptr<AstContext> ctx) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::performClone(llvm::Value* instance, std::shared_ptr<AstContext> ctx) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::performTypeCast(llvm::Value* instance, llvm::Type* targetType, std::shared_ptr<AstContext> ctx) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::resolveTypeMethod(MethodName method, const std::vector<llvm::Value*>& args, std::shared_ptr<AstContext> ctx) noexcept {
        LOG_ERROR("Invoked...................");
        auto& context = ctx->getLLVMContext();
        switch(method) {
            case MethodName::Max: return intMax(context);
            case MethodName::Min: return intMin(context);
            default: return nullptr;
        }
    }

    llvm::Value* IntMethodResolver::resolveInstanceMethod(llvm::Value* instance, const std::vector<llvm::Value*>& args, MethodName method, std::shared_ptr<AstContext> ctx) noexcept {
        LOG_ERROR("Invoked...................");
        Impl impl;
        switch(method) {
            // Arithmetic
            case MethodName::Add: return impl.arithmetic.add(*ctx, instance, args[0]);
            case MethodName::Subtract: return impl.arithmetic.sub(*ctx, instance, args[0]);
            case MethodName::Multiply: return impl.arithmetic.mul(*ctx, instance, args[0]);
            case MethodName::Divide: return impl.arithmetic.div(*ctx,  instance, args[0]);
            case MethodName::Mod: return impl.arithmetic.mod(*ctx, instance, args[0]);
        
            // Bitwise
            case MethodName::BitwiseAnd: return impl.bitwise.bitAnd(*ctx, instance, args[0]);
            case MethodName::BitwiseOr: return impl.bitwise.bitOr(*ctx, instance, args[0]);
            case MethodName::BitwiseXor: return impl.bitwise.bitXor(*ctx, instance, args[0]);
            case MethodName::BitwiseNot: return impl.bitwise.bitNot(*ctx, instance);
            case MethodName::ShiftLeft: return impl.bitwise.shl(*ctx, instance, args[0]);
            case MethodName::ShiftRight: return impl.bitwise.shr(*ctx, instance, args[0]);
        
            // Comparison
            case MethodName::Equals: return impl.cmpStrat.eq(*ctx, instance, args[0]);
            case MethodName::GreaterThan: return impl.cmpStrat.gt(*ctx, instance, args[0]);
            case MethodName::LessThan: return impl.cmpStrat.lt(*ctx, instance, args[0]);
            case MethodName::Compare: return nullptr; // implement as (gt-lt)
        
            // Abs / Clamp / Sign / Even / Odd
            case MethodName::Abs: return impl.absStrat.abs(*ctx, instance);
            case MethodName::Negate: return impl.absStrat.negate(*ctx, instance);
            case MethodName::Sign: return impl.absStrat.sign(*ctx, instance);
            case MethodName::Clamp: return impl.absStrat.clamp(*ctx, instance, args[0], args[1]);
            case MethodName::IsEven: return impl.absStrat.isEven(*ctx, instance);
            case MethodName::IsOdd: return impl.absStrat.isOdd(*ctx, instance);
        
            // Math
            case MethodName::Sqrt: return impl.mathStrat.sqrt(*ctx, instance);
            case MethodName::Pow: return impl.mathStrat.pow(*ctx, instance, args[0]);
        
            // Fixed-point, Overflow, Saturation, Memory
            case MethodName::SAddSat: return impl.saturationStrat.saddSat(*ctx, instance, args[0]);
            case MethodName::UMulWithOverflow: return impl.overflowStrat.umulWithOverflow(*ctx, instance, args[0]);
            case MethodName::Memcpy: return impl.memStrat.memcpy(*ctx, instance, args[0], args[1]);
            
            default: return nullptr;
        }        
    }
}

