#include "userdefined/FileType.hpp"
#include <context/AstContext.hpp>

using namespace LynxContext;

namespace LynxTypes {

    llvm::StructType* FileType::cachedType = nullptr;

    llvm::Type* FileType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::fileType);
            
            std::vector<llvm::Type*> members = {
                llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)), // file path (char*)
                llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context))  // file handle (void*)
            };
            cachedType->setBody(members, false);
        }

        return cachedType;
    }

    llvm::Type* FileType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return llvm::PointerType::getUnqual(this->computeLLVMType());
    }

    llvm::Value* FileType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(computeLLVMType()));
    }

    llvm::Value* FileType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        auto* type = this->computeLLVMType();
        auto var = builder.CreateAlloca(type, nullptr, variableName); 
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::fileType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }  
        
        return var;
    }

    // llvm::Value* FileType::createValue(LValueType value) const {
    //     LOG_INFO("Invoked...");
    //     if (!std::holds_alternative<File>(value)) {
    //         LOG_ERROR("Expected File in LValueType, but got something else.");
    //         return nullptr;
    //     }
    
    //     const File& file = std::get<File>(value);
    //     auto& builder = astContext->getBuilder();
    //     auto& context = astContext->getLLVMContext();
    //     auto* type = computeLLVMType();
    
    //     llvm::Value* fileAlloc = builder.CreateAlloca(type, nullptr, "file_value");
    
    //     llvm::Value* filePathPtr = builder.CreateStructGEP(type, fileAlloc, 0, "path_ptr");
    //     llvm::Value* fileHandlePtr = builder.CreateStructGEP(type, fileAlloc, 1, "handle_ptr");
    
    //     // Convert std::string to LLVM char* (usually via GlobalString)
    //    llvm::Value* llvmStr = builder.CreateGlobalStringPtr(file.path, "file_path");
    //    llvm::Value* nullPtr = llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(context));

    //     builder.CreateStore(llvmStr, filePathPtr);
    //     builder.CreateStore(nullPtr, fileHandlePtr);
    
    //     return fileAlloc;
    // }

    llvm::Value* FileType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool FileType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const FileType*>(other) != nullptr;
    }

    const BaseType* FileType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* FileType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }
    
    std::string FileType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "File";
    }

    llvm::DIType* FileType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        llvm::DIFile* file = builder.createFile("file", "/"); // Dummy file reference

        // Define types for members: `char* path`, `void* handle`
        llvm::DIType* charPtrType = builder.createPointerType(
            builder.createBasicType("char", 8, llvm::dwarf::DW_ATE_signed_char),
            64
        );

        llvm::DIType* voidPtrType = builder.createPointerType(
            builder.createUnspecifiedType("void"),
            64
        );

        std::vector<llvm::Metadata*> elements;

        elements.push_back(builder.createMemberType(
            scope,
            "path",
            file,
            0,
            64, // size in bits
            64, // align
            0,  // offset
            llvm::DINode::FlagZero,
            charPtrType
        ));

        elements.push_back(builder.createMemberType(
            scope,
            "handle",
            file,
            0,
            64,
            64,
            64, // offset of 2nd field (in bits)
            llvm::DINode::FlagZero,
            voidPtrType
        ));

        return builder.createStructType(
            scope,
            getDebugName(), // "File"
            file,
            0,
            getDebugSizeInBits(),
            getDebugAlignInBits(),
            llvm::DINode::FlagZero,
            nullptr,
            builder.getOrCreateArray(elements)
        );
    }

    uint64_t FileType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 128;
    }

    uint32_t FileType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 64;
    }

    llvm::DINode::DIFlags FileType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}