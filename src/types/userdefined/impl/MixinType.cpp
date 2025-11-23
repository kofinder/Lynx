
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "utils/DFSUtils.hpp"
#include "tmpl/TypeCaster.hpp"
#include "userdefined/MixinType.hpp"
#include <constants/LinkageType.hpp>
#include <ast/tmpl/ManglerTemplate.hpp>

namespace LynxTypes {

    using namespace DFSUtils;
    using namespace TypeUtils;
    using namespace LynxAst;

    // NOLINTNEXTLINE(misc-no-recursion)
    llvm::Type* MixinType::computeLLVMType() const {
    
        // Return cached version if already computed
        if (cachedType) return cachedType;
    
        auto& llvmContext = getLLVMContext();
    
        // Step 1: get or create an opaque struct for this mixin
        const auto mixinNameQualified = qualifiedName();
        auto* structType = getOrCreateStruct(llvmContext, mixinNameQualified);
    
        // Register early so recursive/circular references won't break
        registerLLVMType(structType);
        cachedType = structType;
    
        unsigned index = 0;
        fieldNameToIndex.clear();
        std::vector<llvm::Type*> members;

        for (const auto* parent : parentMixins) {
            auto* parentType = llvm::cast<llvm::StructType>(parent->computeLLVMType());
            members.push_back(parentType);
            fieldNameToIndex[parent->mixinName] = index++;
        }

        for (const auto& [name, field] : fields) {
            llvm::Type* fieldLLVMType = nullptr; // NOLINT(misc-const-correctness)
            if (const auto* dateField = TypeCasting::castType<DateTimeType>(field->getType())) {
                fieldLLVMType = dateField->getLLVMPointerType();
            }  else if (const auto* fileField = TypeCasting::castType<MixinType>(field->getType())) {
                fieldLLVMType = fileField->getLLVMPointerType();
            }  else if (const auto* clsField = TypeCasting::castType<ClassType>(field->getType())) {
                fieldLLVMType = clsField->getLLVMPointerType();
            }  else if (const auto* ifaceField = TypeCasting::castType<InterfaceType>(field->getType())) {
                fieldLLVMType = ifaceField->getLLVMPointerType();
            }   else {
                fieldLLVMType = field->getType()->getLLVMType();
            }
            members.push_back(fieldLLVMType);
            fieldNameToIndex[name] = index++;
        }
    
        setStructBodySafe(structType, members);
        return cachedType;
    }    

    llvm::Type* MixinType::getLLVMPointerType() const {
        if (!cachedPointerType) cachedPointerType = llvm::PointerType::get(getLLVMType()->getContext(), 0);
        return cachedPointerType;
    }

    llvm::Value* MixinType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(getLLVMPointerType()));
    }

    llvm::Value* MixinType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), interfaceType));
            var->setMetadata(lynxDataType, metadata);
        }
        return var;
    }

    bool MixinType::isAssignable(BaseType* lhsType, BaseType* rhsType) {
        if (lhsType == rhsType) return true;
        if (auto* lhsMixin = dynamic_cast<MixinType*>(lhsType)) {
            if (auto* rhsClass = dynamic_cast<ClassType*>(rhsType)) {
                return rhsClass->usesMixin(lhsMixin);
            }
        }
        return false;    
    }

    const std::string& MixinType::qualifiedName() const { 
        if (cachedFullName.empty()) cachedFullName = "mixin." + mixinName;
        return cachedFullName;            
    }

    const std::string& MixinType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(mixinName.size());
            for (const unsigned char name : mixinName) {
                cachedLowerName += static_cast<char>(std::tolower(name));
            }
        }
        return cachedLowerName;
    }

    void MixinType::addParentMixin(const MixinType* mixin) {
        for (const auto* existing : parentMixins) if (existing == mixin) return;
        parentMixins.push_back(mixin);        
    }

    void MixinType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (!methods.contains(mangleName)) return;
        methods[mangleName] = std::move(method);
    }

    const MethodType* MixinType::getMethod(const std::string& mangleName) const {
        auto itr = methods.find(mangleName);
        if (itr != methods.end()) return itr->second.get();
        return nullptr;
    }

    bool MixinType::hasMethod(const std::string& mangleName) const {
        return methods.contains(mangleName);
    }

    void MixinType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (!fields.contains(name)) return;
        fields[name] = std::move(field);
    }

    const FieldType* MixinType::getField(const std::string& name) const {
        auto itr = fields.find(name);
        if (itr != fields.end()) return itr->second.get();
        return nullptr;
    }

    bool MixinType::hasField(const std::string& name) const {
        return fields.contains(name);
    }
        
    llvm::Value* MixinType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return getBuilder().CreateStore(rhs, lhs);
    }

    bool MixinType::equals(const BaseType* other) const {
        if (const auto* mixin = dynamic_cast<const MixinType*>(other)) {
            return mixinName == mixin->mixinName;
        }
        return false;
    }

    void MixinType::registerLLVMType(llvm::StructType* structType) const {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    const MixinType* MixinType::fromLLVMType(const llvm::Type* type) {
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

    unsigned MixinType::methodIndex(const std::string& methodName) const {
        auto itr = methodNameToIndex.find(methodName);
        if (itr == methodNameToIndex.end()) throw std::runtime_error("Method not found in vtable: " + methodName);
        return itr->second;
    }

    void MixinType::setFlattenedFields(const std::unordered_map<std::string, FieldType*>& fieldsMap) {
        flattenedFields.clear();
        for (const auto& map : fieldsMap) {
            flattenedFields.push_back(std::unique_ptr<FieldType>(map.second->clone().release()));
        }
    }

    void MixinType::setFlattenedMethods(const std::unordered_map<std::string, MethodType*>& methodsMap, const std::vector<std::string>& methodOrder) {
        flattenedMethods.clear();
        for (const auto& map : methodsMap) {
            flattenedMethods[map.first] = std::unique_ptr<MethodType>(map.second->clone().release());
        }
        flattenedMethodOrder = methodOrder; // optional: preserve order for LLVM struct
    }

    const MethodType* MixinType::getFlattenedMethod(const std::string& sig) const {
        auto itr = flattenedMethods.find(sig);
        return itr != flattenedMethods.end() ? itr->second.get() : nullptr;
    }

    llvm::Value* MixinType::resolveSuperInstanceForMethod(const std::string& methodName, llvm::Value* thisPtr, const std::vector<llvm::Type*>& argTypes) const {
        if(!hasParentMixins()) return nullptr;

         std::unordered_map<const MixinType*, VisitState> state;
         std::vector<const MixinType*> orderedMixins;
     
         for (const auto& mixin : getParents()) topoDFS(mixin, state, orderedMixins);

         for (const MixinType* parent : orderedMixins) {

            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, parent->originalName(), methodName, argTypes);
            if (!parent->hasMethod(mangledName)) continue;

            auto itr = fieldNameToIndex.find(parent->originalName());
            if (itr == fieldNameToIndex.end())  return nullptr;
            
            const unsigned fieldIndex = itr->second;
            auto* llvmStructType = llvm::cast<llvm::StructType>(computeLLVMType());
            auto* superPtr = getBuilder().CreateStructGEP(llvmStructType, thisPtr, fieldIndex, "super_ptr");
            return superPtr;
        }
        
        return nullptr;
    }

    llvm::Value* MixinType::resolveSuperInstanceForField(const std::string& /*fieldName*/) const { return nullptr; }
    std::string MixinType::resolveMethodCall(MethodKind /*kind*/, const std::string& mangledName, const std::vector<llvm::Type*>& /*args*/) const { return mangledName; }

    const BaseType* MixinType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* MixinType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* MixinType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t MixinType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t MixinType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags MixinType::getDIFlags() const { return llvm::DINode::FlagZero; }

    std::unique_ptr<BaseType> MixinType::clone() const {
        using namespace Cloned;
        auto cloned = std::make_unique<MixinType>(getContext(), mixinName);
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->addField(name, std::forward<decltype(field)>(field)); });
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->addMethod(name, std::forward<decltype(method)>(method)); });
        cloneVectorShallow(parentMixins, [&cloned](auto* parent) { cloned->addParentMixin(parent); });
        return cloned;
    }
}