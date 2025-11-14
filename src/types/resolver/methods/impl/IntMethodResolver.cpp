#include "methods/IntMethodResolver.hpp"
#include "utils/TypeNumericUtils.hpp"

namespace LynxTypes {

    llvm::Value* IntMethodResolver::convertToString(AstContext& ctx, llvm::Value* instance) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::performClone(AstContext& ctx, llvm::Value* instance) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::performTypeCast(AstContext& ctx, llvm::Value* instance, llvm::Type* targetType) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* IntMethodResolver::resolveTypeMethod(
        AstContext& ctx,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");
        auto& context = ctx.getLLVMContext();
        if(method == "max") return intMax(context);
        if(method == "min") return intMin(context);
        return nullptr;
    }

    llvm::Value* IntMethodResolver::resolveInstanceMethod(
        AstContext& ctx,
        llvm::Value* instance,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
        // Impl impl;
        // switch(method) {
        //     // Arithmetic
        //     case MethodName::Add: return impl.arithmetic.add(*ctx, instance, args[0]);
        //     case MethodName::Subtract: return impl.arithmetic.sub(*ctx, instance, args[0]);
        //     case MethodName::Multiply: return impl.arithmetic.mul(*ctx, instance, args[0]);
        //     case MethodName::Divide: return impl.arithmetic.div(*ctx,  instance, args[0]);
        //     case MethodName::Mod: return impl.arithmetic.mod(*ctx, instance, args[0]);
        
        //     // Bitwise
        //     case MethodName::BitwiseAnd: return impl.bitwise.bitAnd(*ctx, instance, args[0]);
        //     case MethodName::BitwiseOr: return impl.bitwise.bitOr(*ctx, instance, args[0]);
        //     case MethodName::BitwiseXor: return impl.bitwise.bitXor(*ctx, instance, args[0]);
        //     case MethodName::BitwiseNot: return impl.bitwise.bitNot(*ctx, instance);
        //     case MethodName::ShiftLeft: return impl.bitwise.shl(*ctx, instance, args[0]);
        //     case MethodName::ShiftRight: return impl.bitwise.shr(*ctx, instance, args[0]);
        
        //     // Comparison
        //     case MethodName::Equals: return impl.cmpStrat.eq(*ctx, instance, args[0]);
        //     case MethodName::GreaterThan: return impl.cmpStrat.gt(*ctx, instance, args[0]);
        //     case MethodName::LessThan: return impl.cmpStrat.lt(*ctx, instance, args[0]);
        //     case MethodName::Compare: return nullptr; // implement as (gt-lt)
        
        //     // Abs / Clamp / Sign / Even / Odd
        //     case MethodName::Abs: return impl.absStrat.abs(*ctx, instance);
        //     case MethodName::Negate: return impl.absStrat.negate(*ctx, instance);
        //     case MethodName::Sign: return impl.absStrat.sign(*ctx, instance);
        //     case MethodName::Clamp: return impl.absStrat.clamp(*ctx, instance, args[0], args[1]);
        //     case MethodName::IsEven: return impl.absStrat.isEven(*ctx, instance);
        //     case MethodName::IsOdd: return impl.absStrat.isOdd(*ctx, instance);
        
        //     // Math
        //     case MethodName::Sqrt: return impl.mathStrat.sqrt(*ctx, instance);
        //     case MethodName::Pow: return impl.mathStrat.pow(*ctx, instance, args[0]);
        
        //     // Fixed-point, Overflow, Saturation, Memory
        //     case MethodName::SAddSat: return impl.saturationStrat.saddSat(*ctx, instance, args[0]);
        //     case MethodName::UMulWithOverflow: return impl.overflowStrat.umulWithOverflow(*ctx, instance, args[0]);
        //     case MethodName::Memcpy: return impl.memStrat.memcpy(*ctx, instance, args[0], args[1]);
            
        //     default: return nullptr;
        // }        
    }
}

