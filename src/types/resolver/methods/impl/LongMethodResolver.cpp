#include "methods/LongMethodResolver.hpp"
#include "resolver/TypeStrategyContext.hpp"
#include "resolver/TypeStrategyReflection.hpp"

namespace LynxTypes {

    llvm::Value* LongMethodResolver::resolveTypeMethod(
        const AstContext& ctx,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");
        // if (auto it = staticMethod.find(method); it != staticMethod.end()) {
        //     return it->second(ctx.getLLVMContext());
        // }
        return nullptr;
    }

    llvm::Value* LongMethodResolver::resolveInstanceMethod(
        const AstContext& ctx,
        llvm::Value* instance,
        llvm::Value* instancePtr,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");

        Impl impl;
        StrategyContext stgContext { ctx, instance, instancePtr, args };
        if (auto* value = invokeByName(impl.arithmetic, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.bitwise, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.abs, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.minMax, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.cmp, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.math, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.mem, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.bitManip, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.overflow, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.saturation, method, stgContext)) return value;
        if (auto* value = invokeByName(impl.fixedPoint, method, stgContext)) return value;

        LOG_ERROR("Method not found: ",  method);
        return nullptr;
    }

    llvm::Value* LongMethodResolver::convertToString(const AstContext& ctx, llvm::Value* instance) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* LongMethodResolver::performClone(const AstContext& ctx, llvm::Value* instance) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

    llvm::Value* LongMethodResolver::performTypeCast(const AstContext& ctx, llvm::Value* instance, llvm::Type* targetType) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}