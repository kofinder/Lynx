/**
 * @file TypeConventionTemplate.hpp
 * @brief Defines a set of type conversion utilities for mapping between Lynx AST types
 *        (`BaseType`, `VariableType`, etc.) and LLVM IR types.
 * 
 * This header provides foundational conversion utilities that bridge the Lynx compiler’s 
 * type system with LLVM’s intermediate representation. It enables seamless translation 
 * of high-level language types to their corresponding LLVM types for code generation, 
 * while maintaining flexibility for user-defined and intrinsic types.
 * 
 * **Key Responsibilities:**
 * - Resolve and convert `VariableType` to `BaseType` representations.
 * - Map `BaseType` objects to corresponding `llvm::Type*` constructs.
 * - Handle both built-in and user-defined (custom) types through `AstContext`.
 * - Convert lists of parameters (`Parameter`) to LLVM-compatible type vectors.
 * 
 * **Features:**
 * - Type-safe conversion via template-based parameter handling.
 * - Transparent support for pointer-based LLVM types (`ClassType`, `InterfaceType`, etc.).
 * - Overloaded functions for both `shared_ptr` and raw pointer parameter forms.
 * - Integrates with the `TypeCaster` subsystem for dynamic type resolution.
 * 
 * **Used By:**
 * - Function and method code generation (`FunctionNode`, `ClazzConstructorNode`).
 * - Type checking and symbol resolution subsystems.
 * - LLVM function signature and call instruction generation.
 * 
 * **Functions Overview:**
 * - `convertToBaseRawType` — Returns a raw pointer to a resolved `BaseType`.
 * - `convertToBaseType` — Produces a deep-cloned `BaseType` from a `VariableType`.
 * - `convertToLLVMType` — Converts a `BaseType` or `VariableType` into an LLVM IR type.
 * - `convertToLLVMTypes` — Converts a vector of function parameters into LLVM IR types.
 * 
 * @namespace LynxAst::TypeConv
 * Provides conversion utilities bridging Lynx AST types and LLVM IR types.
 * 
 * @see BaseType, VariableType, AstContext, Parameter, TypeCaster
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
 */

#ifndef LYNX_TYPE_CONVENTION_TEMPLATE_HPP
#define LYNX_TYPE_CONVENTION_TEMPLATE_HPP

#include <vector>
#include <llvm/IR/Value.h>
#include <types/tmpl/TypeCaster.hpp>
#include <types/interfaces/BaseType.hpp>

namespace LynxAst::TypeConv {

    using namespace LynxTypes;

    inline BaseType* convertToBaseRawType(const AstContext& astContext, const VariableType& vType) {
        if(vType.type == DataType::OTHER) {
            auto baseType = astContext.findType(vType.name);
            return baseType.get();
        }
        auto baseType = astContext.findType(vType.type);
        return baseType.get();
    }

    inline std::unique_ptr<BaseType> convertToBaseType(const AstContext& astContext, const VariableType& vType) {
        if(vType.type == DataType::OTHER) {
            auto baseType = astContext.findType(vType.name);
            return baseType->clone();
        }
        auto baseType = astContext.findType(vType.type);
        return baseType->clone();
    }

    inline llvm::Type* convertToLLVMType(const BaseType* baseType) {
        if (auto dateField = TypeCasting::castType<DateTimeType>(baseType)) {
            return dateField->getLLVMPointerType();
        }  else if (auto fileField = TypeCasting::castType<FileType>(baseType)) {
            return fileField->getLLVMPointerType();
        }  else if (auto clsField = TypeCasting::castType<ClassType>(baseType)) {
            return clsField->getLLVMPointerType();
        } else if (auto ifaceField = TypeCasting::castType<InterfaceType>(baseType)) {
            return ifaceField->getLLVMPointerType();
        }

        return baseType->getLLVMType();

    }

    inline llvm::Type* convertToLLVMType(const AstContext& context, const VariableType& vType) {
        std::shared_ptr<BaseType> baseType = nullptr;
        if(vType.type == DataType::OTHER) {
            baseType = context.findType(vType.name);
        } else {
            baseType = context.findType(vType.type);
        }
        return convertToLLVMType(baseType.get());
    }


    /**
     * @brief Convert a vector of Parameter objects (shared_ptr or raw) to LLVM types using AstContext.
     * 
     * @tparam ParamPtr Pointer type (Parameter* or shared_ptr<Parameter>).
     * @param context Pointer to the AST context for type resolution.
     * @param params Vector of parameters to convert.
     * @return std::vector<llvm::Type*> Corresponding LLVM types.
     */
    template <typename ParamPtr>
    inline std::vector<llvm::Type*> convertToLLVMTypes(const AstContext& context, const std::vector<ParamPtr>& params) {
        std::vector<llvm::Type*> types;
        types.reserve(params.size());
        for (const auto& p : params) {
            auto llvmType = convertToLLVMType(context, *p->type);
            types.push_back(llvmType);
        }
        return types;
    }

    /**
     * @brief Convert a pointer-to-vector of Parameter objects to LLVM types using AstContext.
     * 
     * @tparam ParamPtr Pointer type (Parameter* or shared_ptr<Parameter>).
     * @param context Pointer to the AST context for type resolution.
     * @param params Pointer to vector of parameters to convert.
     * @return std::vector<llvm::Type*> Corresponding LLVM types.
     */
    template <typename ParamPtr>
    inline std::vector<llvm::Type*> convertToLLVMTypes(const AstContext& context, const std::vector<ParamPtr>* params) {
        std::vector<llvm::Type*> types;
        types.reserve(params->size());
        for (const auto& p : *params) {
            auto llvmType = convertToLLVMType(context, *p->type);
            types.push_back(llvmType);
        }
        return types;
    }
}

#endif 
