#include "interfaces/BaseType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* BaseType::getLLVMType() const {
        
        if (cachedLLVMType != nullptr) return cachedLLVMType;
    
        auto& context = astContext->getLLVMContext();
    
        cachedLLVMType = llvm::StructType::create(context, "incomplete.llvm.type");
    
        llvm::Type* finalType = computeLLVMType();

        if (!finalType) return cachedLLVMType;

        if (finalType != cachedLLVMType) cachedLLVMType = finalType;

        return cachedLLVMType;
    }

    llvm::LLVMContext& BaseType::getLLVMContext() const noexcept { return astContext->getLLVMContext(); }

    llvm::IRBuilder<>& BaseType::getBuilder() const noexcept { return astContext->getBuilder(); }

    llvm::Module* BaseType::getModule() const noexcept { return astContext->getModule(); }
    
    const std::unordered_map<std::string_view, int>& BaseType::getMethodRegistry() const {
        static const std::unordered_map<std::string_view, int> emptyRegistry;
        return emptyRegistry;
    }   

    BaseType::~BaseType() {
        delete resolver;
        resolver = nullptr;        
    };
}