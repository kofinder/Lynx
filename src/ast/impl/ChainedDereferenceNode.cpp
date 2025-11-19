#include <logger/Logger.hpp>
#include "ChainedDereferenceNode.hpp"
#include "RuntimeClassRegistry.hpp"
#include "utils/VariableUtils.hpp"
#include "RuntimeFunctionRegistry.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {
    
    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxLibRuntime;
    
    llvm::Value* ChainedDereferenceNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Executed ...");
    
        if (derefChain.empty()) {
            LOG_ERROR("Dereference chain is empty.");
            return nullptr;
        }

        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        auto symbol = astContext->getGlobalContext();
    
        // Step 1: Resolve the base variable (e.g., 'this' or an instance)
        auto* baseIdentifier = derefChain[0]->getIdentifierAccessNode();
        auto* currentPtr = baseIdentifier->generateCode(astContext);
        if (!currentPtr) {
            LOG_ERROR("Base variable '{}' not found in the context.", baseIdentifier->getName());
            return nullptr;
        }
    
        // Step 2: Traverse the field chain
        llvm::Type* currentType = currentPtr->getType();
    
        for (size_t i = 1; i < derefChain.size(); ++i) {
            auto* fieldNode = derefChain[i].get();
            std::string fieldName = fieldNode->getIdentifierAccessNode()->getName();
    
            // LLVM 21: If pointer, keep the PointerType, do not call getPointerElementType
            if (auto* ptrType = llvm::dyn_cast<llvm::PointerType>(currentType)) {
                currentType = ptrType; 
            }

            // If current type is not a struct pointer, load the value
            if (!currentType->isStructTy() && currentType->isPointerTy()) {
                currentPtr = builder.CreateLoad(currentType, currentPtr, fieldName + "_load");
                currentType = currentPtr->getType();
            }

            // Ensure it's a struct pointer before field access
            if (!currentType->isPointerTy() && !currentType->isStructTy()) {
                LOG_ERROR("Expected struct type before accessing field '{}'.", fieldName);
                return nullptr;
            }

            // Cast type and find the field index
            auto* structType = llvm::cast<llvm::StructType>(currentType);
            auto* clazzType = ClassType::fromLLVMType(structType);

            if(!clazzType) {
                LOG_ERROR("Struct '{}' not found in Object Context.", fieldName, structType->getName().str());
                return nullptr;
            }

            int fieldIndex = clazzType->getFieldIndex(fieldName);
            if (fieldIndex < 0) {
                LOG_ERROR("Field '{}' not found in struct '{}'.", fieldName, structType->getName().str());
                return nullptr;
            }
    
            // Create GEP to get address of the field
            currentPtr = builder.CreateStructGEP(currentType, currentPtr, fieldIndex, fieldName + "_ptr");
            currentType = currentPtr->getType(); // Update current type to field pointer type
        }
      
        return currentPtr;
    }    

    std::unique_ptr<Node> ChainedDereferenceNode::clone() const {
        return std::make_unique<ChainedDereferenceNode>(Cloneable::cloneNodeVector(derefChain));    
    }
    
}