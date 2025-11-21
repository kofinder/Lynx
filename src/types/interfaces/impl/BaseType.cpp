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
    
    const std::unordered_map<std::string_view, int>& BaseType::getMethodRegistry() const {
        static const std::unordered_map<std::string_view, int> emptyRegistry;
        return emptyRegistry;
    }   

    llvm::Value* BaseType::emitMethodCall(llvm::Value* /*instance*/, llvm::Value* /*instance*/, const std::string& /*instance*/, const std::vector<llvm::Value*>& /*instance*/) {
        return nullptr;
    }

    TypeMethodResolver* BaseType::getOrCreateResolver() const {
        return nullptr;
    }

    BaseType::~BaseType() {
        delete resolver;
        resolver = nullptr;        
    };
}