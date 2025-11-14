#ifndef LYNX_TYPE_METHOD_SUPPORT_HPP
#define LYNX_TYPE_METHOD_SUPPORT_HPP

#include <string>
#include <vector>
#include <llvm/IR/Value.h>

namespace LynxTypes {

    template<typename T>
    concept MethodCapable = requires(T& t, llvm::Value* instance, const std::string& name, const std::vector<llvm::Value*>& args) {
        { t.emitMethodCall(instance, name, args) } -> std::convertible_to<llvm::Value*>;
    };

    template<MethodCapable T>
    constexpr llvm::Value* codegenMethod(T& type, llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) noexcept {
        return type.emitMethodCall(instance, methodName, args);
    }

    // template<typename T>
    // constexpr llvm::Value* codegenMethod(...) noexcept {
    //     static_assert(MethodCapable<T>, "Type does NOT implement: llvm::Value* T::codegenMethod(name, args)");
    //     return nullptr;
    // }

}

#endif
