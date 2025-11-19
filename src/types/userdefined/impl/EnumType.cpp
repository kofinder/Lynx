#include "userdefined/EnumType.hpp"
#include <context/AstContext.hpp>
#include "utils/TypeUtils.hpp"
#include "resolver/methods/EnumMethodResolver.hpp"

namespace LynxTypes {
    
    using namespace LynxContext;

    llvm::Type* EnumType::computeLLVMType() const {
        LOG_INFO("Invoked...");
    
        using namespace TypeUtils;
        if (cachedType) return cachedType;
    
        std::string fullName = qualifiedName();
        auto& llvmContext = astContext->getLLVMContext();
    
        // Step 1: get or create opaque struct
        auto* structType = getOrCreateStruct(llvmContext, fullName);
        const_cast<EnumType*>(this)->registerLLVMType(structType);

        // Step 2: Build union type safely
        auto* unionType = createEnumValueUnion(llvmContext);
    
        // Step 3: Set the body of the enum struct
        std::vector<llvm::Type*> elementMembers = { llvm::Type::getInt64Ty(llvmContext),/* tag */ unionType /* payload */ };
        structType->setBody(elementMembers, false);
    
        return structType;
    }
    
    
    llvm::StructType* EnumType::createEnumValueUnion(llvm::LLVMContext& context) const {
        if (cachedUnionType) return cachedUnionType;
    
        std::string payloadName = qualifiedName() + ".payload";
    
        llvm::StructType* structType = nullptr;
        if (auto* existing = llvm::StructType::getTypeByName(context, payloadName)) {
            structType = existing;
        } else {
            structType = llvm::StructType::create(context, payloadName);
        }
        cachedUnionType = structType;
    
        // Now set the members
        std::vector<llvm::Type*> members = {
            llvm::PointerType::get(context, 0),  // e.g., pointer payload
            llvm::Type::getInt8Ty(context),     // example 1-byte field
            llvm::Type::getInt64Ty(context)     // example 8-byte field
        };
        structType->setBody(members, false);
    
        return structType;
    }

    void EnumType::registerLLVMType(llvm::StructType* structType) {
        if (!structType) return;
        cachedType = structType;
        llvmTypeToClass[structType] = this;
    }

    EnumType* EnumType::fromLLVMType(const llvm::Type* type) {
        if (!type) return nullptr;
        // if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {
        //     type = ptrType->getPointerElementType();
        // }
        if (auto structType = llvm::dyn_cast<llvm::StructType>(type)) {
            auto it = llvmTypeToClass.find(structType);
            if (it != llvmTypeToClass.end())  return it->second;
        }
        return nullptr;
    }

    llvm::Type* EnumType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        if (cachedPointerType) return cachedPointerType;
        cachedPointerType = llvm::PointerType::get(computeLLVMType()->getContext(), 0);
        return cachedPointerType;

    }

    llvm::Value* EnumType::getDefaultValue() {
        LOG_INFO("Invoked...");
        auto* type = computeLLVMType();
        auto& ctx = astContext->getLLVMContext();
        auto* structType = llvm::cast<llvm::StructType>(type);

        std::vector<llvm::Constant*> values;
        if (structType->getElementType(0)->isIntegerTy()) {
            values.push_back(llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 0));
            values.push_back(llvm::ConstantPointerNull::get(llvm::PointerType::get(ctx, 0)));
        } else {
            values.push_back(llvm::ConstantPointerNull::get(llvm::PointerType::get(ctx, 0)));
            values.push_back(llvm::ConstantPointerNull::get(llvm::PointerType::get(ctx, 0)));
        }

        return llvm::ConstantStruct::get(structType, values);
    }

    llvm::Value* EnumType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* enumType = computeLLVMType();
        auto var = builder.CreateAlloca(enumType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureEnumType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* EnumType::assignTo(llvm::Value* syntaxAlloca, llvm::Value* valuePtr) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
        auto* module  = astContext->getModule();
        auto* int8PtrTy = llvm::PointerType::get(context, 0);

        // Cast both source and destination to i8*
        llvm::Value* destPtr = builder.CreateBitCast(syntaxAlloca, int8PtrTy);
        llvm::Value* srcPtr  = builder.CreateBitCast(valuePtr, int8PtrTy);

        uint64_t sizeInBytes = module->getDataLayout().getTypeAllocSize(computeLLVMType());

        // Get memcpy intrinsic declaration (4-arg version)
        llvm::Function* memcpyFunc = llvm::Intrinsic::getOrInsertDeclaration(
            module, llvm::Intrinsic::memcpy,
            {int8PtrTy, int8PtrTy, builder.getInt64Ty() }
        );

        // Call llvm.memcpy with 4 arguments: dest, src, size, isVolatile
        builder.CreateCall(memcpyFunc, {
            destPtr,
            srcPtr,
            llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), sizeInBytes),
            llvm::ConstantInt::getFalse(context)
        });

        return nullptr;
    }

    // std::unique_ptr<TypeMethodResolver> EnumType::getOrCreateResolver() const {
    //     return std::make_unique<EnumMethodResolver>();
    // }

    const BaseType* EnumType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* EnumType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }
    
    bool EnumType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const EnumType*>(other) != nullptr;
    }
    
    const std::string& EnumType::qualifiedName() const { 
        if (cachedFullName.empty()) {
            cachedFullName = "enum." + enumName;
        }
        return cachedFullName;            
    }

    std::string EnumType::getDebugName() const {
        return qualifiedName();
    }

    llvm::DIType* EnumType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        return builder.createBasicType(
            getDebugName(),            // "Enum"
            getDebugSizeInBits(),      // 32 bits
            llvm::dwarf::DW_ATE_signed // Signed integer representation
        );
    }

    uint64_t EnumType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 32;
    }

    uint32_t EnumType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32;
    }

    llvm::DINode::DIFlags EnumType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }

    void EnumType::addMember(const std::string& name, EnumMember member) {
        members.emplace(name, member);
    }

    std::optional<EnumMember> EnumType::getMember(const std::string& name) const {
        auto it = members.find(name);
        if (it != members.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::unique_ptr<BaseType> EnumType::clone() const {
        auto cloned = std::make_unique<EnumType>(astContext, enumName);
        for (const auto& [key, member] : members) {
            cloned->addMember(key, member);
        }
        return cloned;
    }
}