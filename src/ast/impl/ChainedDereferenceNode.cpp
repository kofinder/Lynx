#include <logger/Logger.hpp>
#include "ChainedDereferenceNode.hpp"
#include "RuntimeClassRegistry.hpp"
#include "utils/VariableUtils.hpp"
#include "RuntimeFunctionRegistry.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxLibRuntime;

namespace LynxAst {

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
    
            // If pointer, get the pointed-to type
            if (currentType->isPointerTy()) {
                currentType = currentType->getPointerElementType();
            }
    
            // Load value if current type is not a struct
            if (!currentType->isStructTy()) {
                currentPtr = builder.CreateLoad(currentType, currentPtr, fieldName + "_load");
                currentType = currentPtr->getType();
                if (currentType->isPointerTy()) {
                    currentType = currentType->getPointerElementType();
                }
            }
    
            // Ensure it's a struct before field access
            if (!currentType->isStructTy()) {
                LOG_ERROR("Expected struct type before accessing field '{}'.", fieldName);
                return nullptr;
            }
    
            // Cast type and find the field index
            auto* structType = llvm::cast<llvm::StructType>(currentType);
            std::cout << "struct Type name ===>: " << structType->getName().str() << std::endl;
            // auto objCtxType = symbol->findObjectType(structType->getName().str());
            // if(!objCtxType) {
            //     LOG_ERROR("Struct '{}' not found in Object Context.", fieldName, structType->getName().str());
            //     return nullptr;
            // }

            // int fieldIndex = objCtxType->getFieldIndex(fieldName);
            // if (fieldIndex < 0) {
            //     LOG_ERROR("Field '{}' not found in struct '{}'.", fieldName, structType->getName().str());
            //     return nullptr;
            // }
    
            // // Create GEP to get address of the field
            // currentPtr = builder.CreateStructGEP(currentType, currentPtr, fieldIndex, fieldName + "_ptr");
            // currentType = currentPtr->getType(); // Update current type to field pointer type
        }
      
        return currentPtr;
    }    

    std::unique_ptr<Node> ChainedDereferenceNode::clone() const {
        return std::make_unique<ChainedDereferenceNode>(Cloneable::cloneNodeVector(derefChain));    
    }
    
}