#include "methods/FloatMethodResolver.hpp"
#include "resolver/TypeStrategyContext.hpp"
#include "resolver/TypeStrategyReflection.hpp"

namespace LynxTypes {

    llvm::Value* FloatMethodResolver::resolveInstanceMethod(
        const AstContext& ctx,
        llvm::Value* instance,
        llvm::Value* instancePtr,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        
        const Impl impl;
        const StrategyContext stgContext {     
            .ctx = ctx,
            .instance = instance,
            .instancePtr = instancePtr,
            .args = args
         };
        
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

    llvm::Value* FloatMethodResolver::resolveTypeMethod(
        const AstContext& /*ctx*/,
        const std::string& /*method*/, 
        const std::vector<llvm::Value*>& /*args*/
    ) noexcept { return nullptr; }
    llvm::Value* FloatMethodResolver::convertToString(const AstContext& /*ctx*/, llvm::Value* /*instance*/) noexcept { return nullptr; }
    llvm::Value* FloatMethodResolver::performClone(const AstContext& /*ctx*/, llvm::Value* /*instance*/) noexcept { return nullptr; }
    llvm::Value* FloatMethodResolver::performTypeCast(const AstContext& /*ctx*/, llvm::Value* /*instance*/, llvm::Type* /*llvmType*/) noexcept { return nullptr; }

}