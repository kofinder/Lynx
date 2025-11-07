/**
 * @file LLVMManglarTemplate.hpp
 * @brief Declares name mangling and demangling utilities for functions and 
 *        classes within the Lynx compiler’s LLVM backend.
 * 
 * The LLVMManglarTemplate utilities provide a unified interface for generating 
 * and decoding mangled symbol names following the Itanium C++ ABI scheme. 
 * These tools are essential for ensuring consistent symbol resolution across 
 * function overloads, class methods, and constructors during code generation 
 * and linkage.
 * 
 * **Key Responsibilities:**
 * - Generate mangled names for functions, member functions, and constructors.
 * - Support both `BaseType` and `llvm::Type` parameter signatures.
 * - Provide generic and type-safe demangling utilities for class and method names.
 * - Integrate with the `ManglingEngine` for Itanium ABI-compliant encoding.
 * 
 * **Used By:**
 * - Code generation and linking stages.
 * - Symbol resolution, debugging, and reflection mechanisms.
 * 
 * @see ManglingEngine, MangleParameter, BaseType, llvm::Type
 * 
 * @note All mangling operations default to the Itanium ABI convention for 
 *       cross-platform consistency. Demangling utilities support both 
 *       class-level and method-level extraction.
 * 
 * @namespace LynxAst::Mangle
 * Provides a collection of utilities for symbol name mangling and demangling 
 * within the Lynx compiler's LLVM-based backend.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LLVM_MANGLAR_TEMPLATE_HPP
#define LYNX_LLVM_MANGLAR_TEMPLATE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cxxabi.h>
#include <optional>
#include <llvm/IR/Function.h>
#include <mangler/ManglingEngine.hpp>
#include <constants/DataType.hpp>
#include <constants/Parameter.hpp>
#include <types/tmpl/TypeChecker.hpp>

namespace LynxAst::Mangle {

    using namespace LynxTypes;
    using namespace LynxMangler;
    using namespace LynxConstants;

    inline std::string get(ManglerKind kind, const std::string& className) {
        ManglingEngine engine(ManglerType::ITANIUM);
        const std::vector<std::string> emptyParams;
        return engine.mangle(MangleParameter::makeClass(className));
    }

    inline std::string get(ManglerKind kind, const std::string& funcName, const std::vector<BaseType*>& params) {
        ManglingEngine engine(ManglerType::ITANIUM);
        if(kind == ManglerKind::CONSTRUCTOR) {
            return engine.mangle(MangleParameter::makeConstructor(funcName, std::cref(params)));
        }
        return engine.mangle(MangleParameter::makeFunction(funcName, std::cref(params)));
    }

    inline std::string get(ManglerKind kind, const std::string& funcName, const std::vector<llvm::Type*>& params) {
        ManglingEngine engine(ManglerType::ITANIUM);
        if(kind == ManglerKind::CONSTRUCTOR) {
            return engine.mangle(MangleParameter::makeConstructor(funcName, std::cref(params)));
        }
        return engine.mangle(MangleParameter::makeFunction(funcName, std::cref(params)));
    }

    inline std::string get(ManglerKind kind, const std::string& className, const std::string& funcName, const std::vector<BaseType*>& params) {
        ManglingEngine engine(ManglerType::ITANIUM);
        return engine.mangle(MangleParameter::makeMemberFunction(className, funcName, std::cref(params)));
    }

    inline std::string get(ManglerKind kind, const std::string& className, const std::string& funcName, const std::vector<llvm::Type*>& params) {
        ManglingEngine engine(ManglerType::ITANIUM);
        return engine.mangle(MangleParameter::makeMemberFunction(className, funcName, std::cref(params)));
    }

    /**
     * @brief Generic demangle function to extract method or class name.
     */
    enum class DemanglePart { Class, Method };
    template <DemanglePart Part, typename StringType>
    inline std::string demangle(const StringType& mangledName) {
        std::string str = std::string(mangledName);
        int status = 0;
        char* demangled = abi::__cxa_demangle(str.c_str(), nullptr, nullptr, &status);
        std::string result = (status == 0 && demangled) ? demangled : str;
        free(demangled);

        size_t pos = result.rfind("::");
        if constexpr (Part == DemanglePart::Method) {
            return (pos == std::string::npos) ? result : result.substr(pos + 2);
        } else { 
            return (pos == std::string::npos) ? "" : result.substr(0, pos);
        }
    }

    /**
     * @brief Convenience functions for explicit method or class extraction.
     */
    template <typename StringType>
    inline std::string demangleToMethod(const StringType& mangledName) {
        return demangle<DemanglePart::Method>(mangledName);
    }

    template <typename StringType>
    inline std::string demangleToClass(const StringType& mangledName) {
        return demangle<DemanglePart::Class>(mangledName);
    }
}

#endif
