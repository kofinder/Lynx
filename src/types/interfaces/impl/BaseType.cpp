#include "interfaces/BaseType.hpp"
#include "visitor/TypeVisitor.hpp"

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
    
    const std::unordered_map<std::string, int>& BaseType::getStaticMethodRegistry() const {
        return {}; 
    }   

    const std::unordered_map<std::string, int>& BaseType::getInstanceMethodRegistry() const {
        return {}; 
    }   

    llvm::Value* BaseType::codegenStaticMethod(const std::string& methodName, const std::vector<llvm::Value*>& args) {
        return nullptr;
    }
}