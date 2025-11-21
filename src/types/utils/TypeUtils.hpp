/**
 * @file TypeUtils.hpp
 * @brief Utility functions for handling LLVM and Lynx type conversions and operations.
 *
 * This header provides helper functions for creating and managing LLVM `StructType`s,
 * safely setting struct bodies, appending non-null types to containers, and converting
 * LLVM types to Lynx `BaseType` instances, including user-defined classes, interfaces,
 * mixins, and enums.
 *
 * These utilities simplify type management during code generation and type resolution
 * in the Lynx compiler.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_UTILS_HPP
#define LYNX_TYPE_UTILS_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <types/userdefined/ClassType.hpp>
#include <types/userdefined/InterfaceType.hpp>
#include <types/userdefined/MixinType.hpp>
#include <types/userdefined/EnumType.hpp>

namespace LynxTypes::TypeUtils {

    inline llvm::StructType* getOrCreateStruct(llvm::LLVMContext& ctx, const std::string& name) {
        if (auto* structType = llvm::StructType::getTypeByName(ctx, name))  return structType;
        return llvm::StructType::create(ctx, name);
    }
        
    inline void setStructBodySafe(llvm::StructType* structType, const std::vector<llvm::Type*>& elements) {
        if (!structType->isOpaque()) return;
        structType->setBody(elements, false);
    }

    template <typename TContainer, typename Func>
    void appendIfNotNull(TContainer& dest, const TContainer& src, Func getType) {
        for (auto* ptr : src) {
            if (ptr) dest.push_back(getType(ptr));
        }
    } 
    
    inline const BaseType* convertLLVMTypeToBaseType(llvm::Type* llvmType, AstContext& context) {
        if(auto clazzType = ClassType::fromLLVMType(llvmType)) { 
            return clazzType;
        } else if(auto ifaceType = InterfaceType::fromLLVMType(llvmType)) {
            return ifaceType;
        } else if(auto mixinType = MixinType::fromLLVMType(llvmType)) {
            return mixinType;
        } else if(auto enumType = EnumType::fromLLVMType(llvmType)) {
           return enumType;
        }  else {
            auto basePtrType = context.findType("llvmType");
            return basePtrType.get();
        }
    }
}

#endif
