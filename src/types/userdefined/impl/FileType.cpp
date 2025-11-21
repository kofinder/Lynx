#include "userdefined/FileType.hpp"
#include <context/AstContext.hpp>

using namespace LynxContext;

namespace LynxTypes {

    llvm::StructType* FileType::cachedType = nullptr;

    llvm::Type* FileType::computeLLVMType() const {
        
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::fileType);
            
            std::vector<llvm::Type*> members = {
                llvm::PointerType::get(llvm::Type::getInt8Ty(context)->getContext(), 0), // file path (char*)
                llvm::PointerType::get(llvm::Type::getInt8Ty(context)->getContext(), 0)  // file handle (void*)
            };
            cachedType->setBody(members, false);
        }

        return cachedType;
    }

    llvm::Type* FileType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* FileType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::PointerType::get(computeLLVMType()->getContext(), 0));
    }

    llvm::Value* FileType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        auto* type = this->computeLLVMType();
        auto* var = builder.CreateAlloca(type, nullptr, variableName); 
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::fileType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }  
        
        return var;
    }

    llvm::Value* FileType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool FileType::equals(const BaseType* other) const {
        return dynamic_cast<const FileType*>(other) != nullptr;
    }

    const BaseType* FileType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* FileType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* FileType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t FileType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t FileType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags FileType::getDIFlags() const { return llvm::DINode::FlagZero; }
    
}