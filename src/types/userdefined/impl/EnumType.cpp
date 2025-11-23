#include "userdefined/EnumType.hpp"
#include <context/AstContext.hpp>
#include "utils/TypeUtils.hpp"
#include "resolver/methods/EnumMethodResolver.hpp"

namespace LynxTypes {
    
    using namespace LynxContext;

    llvm::Type* EnumType::computeLLVMType() const {
    
        using namespace TypeUtils;
        if (cachedType) return cachedType;
    
        auto& llvmContext = getLLVMContext();
    
        // Step 1: get or create opaque struct
        const auto fullName = qualifiedName();
        auto* structType = getOrCreateStruct(llvmContext, fullName);
        registerLLVMType(structType);

        // Step 2: Build union type safely
        auto* unionType = createEnumValueUnion(llvmContext);
    
        // Step 3: Set the body of the enum struct
        const std::vector<llvm::Type*> elementMembers = { llvm::Type::getInt64Ty(llvmContext),/* tag */ unionType /* payload */ };
        structType->setBody(elementMembers, false);
    
        return structType;
    }
    
    
    llvm::StructType* EnumType::createEnumValueUnion(llvm::LLVMContext& context) const {
        if (cachedUnionType) return cachedUnionType;
    
        const auto payloadName = qualifiedName() + ".payload";
        llvm::StructType* structType = nullptr;
        if (auto* existing = llvm::StructType::getTypeByName(context, payloadName)) {
            structType = existing;
        } else {
            structType = llvm::StructType::create(context, payloadName);
        }
        cachedUnionType = structType;
    
        // Now set the members
        const std::vector<llvm::Type*> members = {
            llvm::PointerType::get(context, 0),  // e.g., pointer payload
            llvm::Type::getInt8Ty(context),     // example 1-byte field
            llvm::Type::getInt64Ty(context)     // example 8-byte field
        };
        structType->setBody(members, false);
    
        return structType;
    }

    void EnumType::registerLLVMType(llvm::StructType* structTy) const {
        if (!structType) return;
        cachedType = structTy;
        llvmTypeToClass[structTy] = this;
    }

    const EnumType* EnumType::fromLLVMType(const llvm::Type* type) {
        if (!type) return nullptr;
        // if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {
        //     type = ptrType->getPointerElementType();
        // }
        if (const auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            auto itr = llvmTypeToClass.find(structType);
            if (itr != llvmTypeToClass.end())  return itr->second;
        }
        return nullptr;
    }

    llvm::Type* EnumType::getLLVMPointerType() const {
        if (cachedPointerType) return cachedPointerType;
        cachedPointerType = llvm::PointerType::get(computeLLVMType()->getContext(), 0);
        return cachedPointerType;
    }

    llvm::Value* EnumType::getDefaultValue() {
        auto* type = computeLLVMType();
        auto& ctx = getLLVMContext();
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

    llvm::Value* EnumType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureEnumType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* EnumType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = getBuilder();
        auto& context = getLLVMContext();
        auto* module  = getModule();
        auto* int8PtrTy = llvm::PointerType::get(context, 0);

        // Cast both source and destination to i8*
        auto* destPtr = builder.CreateBitCast(lhs, int8PtrTy);
        auto* srcPtr  = builder.CreateBitCast(rhs, int8PtrTy);

        const uint64_t sizeInBytes = module->getDataLayout().getTypeAllocSize(computeLLVMType());

        // Get memcpy intrinsic declaration (4-arg version)
        auto* memcpyFunc = llvm::Intrinsic::getOrInsertDeclaration(
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

    bool EnumType::equals(const BaseType* other) const {
        return dynamic_cast<const EnumType*>(other) != nullptr;
    }
    
    const std::string& EnumType::qualifiedName() const { 
        if (cachedFullName.empty()) cachedFullName = "enum." + enumName;
        return cachedFullName;            
    }

    void EnumType::addMember(const std::string& name, const EnumMember& member) {
        members.emplace(name, member);
    }

    std::optional<EnumMember> EnumType::getMember(const std::string& name) const {
        auto itr = members.find(name);
        if (itr != members.end()) return itr->second;
        return std::nullopt;
    }

    const BaseType* EnumType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* EnumType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* EnumType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t EnumType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t EnumType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags EnumType::getDIFlags() const { return llvm::DINode::FlagZero; }
    
    std::unique_ptr<BaseType> EnumType::clone() const {
        auto cloned = std::make_unique<EnumType>(getContext(), enumName);
        for (const auto& [key, member] : members) {
            cloned->addMember(key, member);
        }
        return cloned;
    }
}