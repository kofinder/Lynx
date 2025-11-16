#include "methods/IntMethodResolver.hpp"
#include "utils/TypeNumericUtils.hpp"
#include "utils/TypeResolverConstant.hpp"
#include "utils/TypeResolverWrapper.hpp"
#include "resolver/TypeStrategyReflection.hpp"

namespace LynxTypes {

    llvm::Value* IntMethodResolver::resolveTypeMethod(AstContext& ctx, const std::string& method, const std::vector<llvm::Value*>& args) noexcept {
        LOG_ERROR("Invoked...................");
        // if (auto it = staticMethod.find(method); it != staticMethod.end()) {
        //     return it->second(ctx.getLLVMContext());
        // }
        return nullptr;
    }

    llvm::Value* IntMethodResolver::resolveInstanceMethod(AstContext& ctx, llvm::Value* instance, const std::string& method, const std::vector<llvm::Value*>& args) noexcept {
        LOG_ERROR("Invoked...................");

        Impl impl;
        StrategyContext stgContext { ctx, instance, args };
        if (auto* value = invokeByName(impl.arithmetic, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.bitwise, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.absStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.minMaxStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.cmpStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.mathStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.memStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.bitManipStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.overflowStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.saturationStrat, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.fixedPointStrat, method, stgContext)) return value;

        LOG_ERROR("Method not found: ",  method);
        return nullptr;
    }

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
}




       

