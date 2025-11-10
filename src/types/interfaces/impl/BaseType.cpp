#include "interfaces/BaseType.hpp"
#include <resolver/TypeVisitor.hpp>

#include <resolver/TypeMethodResolver.hpp>

namespace LynxTypes {

    llvm::Type* BaseType::getLLVMType() const {
        if (cachedLLVMType != nullptr) return cachedLLVMType;
    
        auto& context = astContext->getLLVMContext();
    
        // Assign placeholder to avoid recursion
        cachedLLVMType = llvm::StructType::create(context, "incomplete.llvm.type");
    
        llvm::Type* finalType = computeLLVMType();

        if (!finalType) {
            return cachedLLVMType;
        }    

        if (finalType != cachedLLVMType) cachedLLVMType = finalType;

        return cachedLLVMType;
    }
    
    std::unique_ptr<LynxResolver::TypeMethodResolver> BaseType::createMethodResolver() const {
        return nullptr; // default: no method resolver
    }    
}