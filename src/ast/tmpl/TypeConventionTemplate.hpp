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
