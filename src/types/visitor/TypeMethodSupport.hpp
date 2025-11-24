/**
 * @file TypeMethodSupport.hpp
 * @brief Provides helper utilities for invoking methods on Lynx types.
 *
 * This header defines concepts and helper functions to standardize how
 * methods are emitted for different Lynx types in LLVM IR.
 *
 * Key components:
 *  - `MethodCapable` concept: ensures a type implements `emitMethodCall()`
 *    with the correct signature.
 *  - `codegenMethod()` function: a constexpr helper that forwards method
 *    calls to a type's `emitMethodCall()`, simplifying code generation.
 *
 * Using these helpers ensures:
 *  - compile-time safety for method invocation on types,
 *  - consistent LLVM IR generation across all type implementations,
 *  - reduced boilerplate when emitting method calls in the compiler backend.
 *
 * This design follows the pattern of CRTP-style or concept-based
 * polymorphism, avoiding dynamic dispatch while retaining flexibility.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:   Jan 2, 2022
*/

#ifndef LYNX_TYPE_METHOD_SUPPORT_HPP
#define LYNX_TYPE_METHOD_SUPPORT_HPP

#include <string>
#include <vector>
#include <llvm/IR/Value.h>

namespace LynxTypes {

    template<typename T>
    concept MethodCapable = requires(T& instance, llvm::Value* inst, llvm::Value* instPtr, const std::string& name, const std::vector<llvm::Value*>& args) {
        { instance.emitMethodCall(inst, instPtr, name, args) } -> std::convertible_to<llvm::Value*>;
    };

    template<MethodCapable T>
    constexpr llvm::Value* codegenMethod(T& instance, llvm::Value* inst, llvm::Value* instPtr, const std::string& methodName, const std::vector<llvm::Value*>& args) noexcept {
        return instance.emitMethodCall(inst, instPtr, methodName, args);
    }

}

#endif
