
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "utils/DFSUtils.hpp"
#include "tmpl/TypeCaster.hpp"
#include "userdefined/MixinType.hpp"
#include <constants/LinkageType.hpp>
#include <ast/tmpl/ManglerTemplate.hpp>

namespace LynxTypes {

    using namespace DFSUtils;
    using namespace LynxAst;

    llvm::Type* MixinType::computeLLVMType() const {
        using namespace TypeUtils;
    
        // Return cached version if already computed
        if (cachedType) return cachedType;
    
        std::string ifaceName = qualifiedName();
        auto& llvmContext = astContext->getLLVMContext();
    
        // Step 1: get or create an opaque struct for this mixin
        auto* structType = getOrCreateStruct(llvmContext, ifaceName);
    
        // Register early so recursive/circular references won't break
        const_cast<MixinType*>(this)->registerLLVMType(structType);
        cachedType = structType;
    
        unsigned index = 0;
        fieldNameToIndex.clear();
        std::vector<llvm::Type*> members;

        for (const auto* parent : parentMixins) {
            auto* parentType = llvm::cast<llvm::StructType>(parent->computeLLVMType());
            members.push_back(parentType);
            fieldNameToIndex[parent->mixinName] = index++;
        }

        // Add this mixin's own flattened fields
        for (const auto& [name, field] : fields) {
            llvm::Type* ty = nullptr;
            if (auto dateField = TypeCasting::castType<DateTimeType>(field->getType())) {
                ty = dateField->getLLVMPointerType();
            }  else if (auto fileField = TypeCasting::castType<FileType>(field->getType())) {
                ty = fileField->getLLVMPointerType();
            }  else if (auto clsField = TypeCasting::castType<ClassType>(field->getType())) {
                ty = clsField->getLLVMPointerType();
            }  else if (auto ifaceField = TypeCasting::castType<InterfaceType>(field->getType())) {
                ty = ifaceField->getLLVMPointerType();
            }   else {
                ty = field->getType()->getLLVMType();
            }
        
            members.push_back(ty);
            fieldNameToIndex[name] = index++;
        }
    
        // Step 4: finalize the struct body
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

    llvm::Value* MixinType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        auto llvmType = computeLLVMType();
        auto var = builder.CreateAlloca(llvmType, nullptr, variableName);
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
        if (cachedFullName.empty()) {
            cachedFullName = "mixin." + mixinName;
        }
        return cachedFullName;            
    }

    const std::string& MixinType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(mixinName.size());
            for (char c : mixinName) {
                cachedLowerName += std::tolower(static_cast<unsigned char>(c));
            }
        }
        return cachedLowerName;    
    }

    void MixinType::addParentMixin(const MixinType* mixin) {
        for (const auto* existing : parentMixins) {
            if (existing == mixin) return;
        }
        parentMixins.push_back(mixin);        
    }

    void MixinType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (methods.find(mangleName) != methods.end()) {
            std::cerr << "Warning: Method '" << mangleName << "' already exists in interface '" << mixinName << "'\n";
            return;
        }
        methods[mangleName] = std::move(method);
    }

    const MethodType* MixinType::getMethod(const std::string& mangleName) const {
        auto it = methods.find(mangleName);
        if (it != methods.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool MixinType::hasMethod(const std::string& mangleName) const {
        return methods.find(mangleName) != methods.end();
    }

    // std::unique_ptr<TypeMethodResolver> MixinType::getOrCreateResolver() const {
    //     return std::make_unique<MixinMethodResolver>();
    // }

    void MixinType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (fields.find(name) != fields.end()) {
            std::cerr << "Warning: Field '" << name << "' already exists in interface '" << mixinName << "'\n";
            return;
        }
        fields[name] = std::move(field);
    }

    const FieldType* MixinType::getField(const std::string& name) const {
        auto it = fields.find(name);
        if (it != fields.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool MixinType::hasField(const std::string& name) const {
        return fields.find(name) != fields.end();
    }
        
    llvm::Value* MixinType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* MixinType::createWithStatic(bool newIsStatic) const {
        return this;
    }

    const BaseType* MixinType::createWithConst(bool newIsConst) const {
        return this;
    }

    bool MixinType::equals(const BaseType* other) const {
        if (auto* iface = dynamic_cast<const MixinType*>(other)) {
            return this->mixinName == iface->mixinName;
        }
        return false;
    }

    std::string MixinType::getDebugName() const { return qualifiedName(); }

    llvm::DIType* MixinType::getDIType(llvm::DIScope* scope) const {
        return nullptr;
    }

    uint64_t MixinType::getDebugSizeInBits() const {
        return 64; 
    }

    uint32_t MixinType::getDebugAlignInBits() const {
        return 8;
    }

    llvm::DINode::DIFlags MixinType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    } 

    void MixinType::registerLLVMType(llvm::StructType* structType) {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    MixinType* MixinType::fromLLVMType(const llvm::Type* type) {
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

    unsigned MixinType::methodIndex(const std::string& methodName) const {
        auto it = methodNameToIndex.find(methodName);
        if (it == methodNameToIndex.end()) {
            LOG_ERROR("Method not found in vtable: {}", methodName);
            throw std::runtime_error("Method not found in vtable: " + methodName);
        }
        return it->second;
    }

    void MixinType::setFlattenedFields(const std::unordered_map<std::string, FieldType*>& fieldsMap) {
        flattenedFields.clear();
        for (const auto& kv : fieldsMap) {
            flattenedFields.push_back(std::unique_ptr<FieldType>(kv.second->clone().release()));
        }
    }

    void MixinType::setFlattenedMethods(const std::unordered_map<std::string, MethodType*>& methodsMap, const std::vector<std::string>& methodOrder) {
        flattenedMethods.clear();
        for (const auto& kv : methodsMap) {
            flattenedMethods[kv.first] = std::unique_ptr<MethodType>(kv.second->clone().release());
        }
        flattenedMethodOrder = methodOrder; // optional: preserve order for LLVM struct
    }

    const MethodType* MixinType::getFlattenedMethod(const std::string& sig) const {
        auto it = flattenedMethods.find(sig);
        return it != flattenedMethods.end() ? it->second.get() : nullptr;
    }

    llvm::Value* MixinType::resolveSuperInstanceForMethod(const std::string& methodName, llvm::Value* thisPtr, std::vector<llvm::Type*> argTypes) const {
        if(!hasParentMixins()) return nullptr;

        LOG_INFO("Resolving super instance for method '{}' in '{}'", methodName, this->qualifiedName());

         std::unordered_map<const MixinType*, VisitState> state;
         std::vector<const MixinType*> orderedMixins;
     
         for (const auto& mixin : getParents()) topoDFS(mixin, state, orderedMixins);

         for (const MixinType* parent : orderedMixins) {

            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, parent->originalName(), methodName, argTypes);
            if (!parent->hasMethod(mangledName)) continue;

            auto it = fieldNameToIndex.find(parent->originalName());
            if (it == fieldNameToIndex.end()) {
                LOG_ERROR("Parent '{}' not found in struct layout!", parent->qualifiedName());
                return nullptr;
            }
            
            unsigned fieldIndex = it->second;
            auto& builder = astContext->getBuilder();
            auto* llvmStructType = llvm::cast<llvm::StructType>(computeLLVMType());
            llvm::Value* superPtr = builder.CreateStructGEP(llvmStructType, thisPtr, fieldIndex, "super_ptr");

            return superPtr;
        }
        
        return nullptr;
    }

    llvm::Value* MixinType::resolveSuperInstanceForField(const std::string& fieldName) const {
        return nullptr;
    }

    std::string MixinType::resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const {
        return mangledName;
    }


    std::unique_ptr<BaseType> MixinType::clone() const {
        using namespace Cloned;
        auto cloned = std::make_unique<MixinType>(astContext, mixinName);
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->addField(name, std::move(field)); });
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->addMethod(name, std::move(method)); });
        cloneVectorShallow(parentMixins, [&cloned](auto* parent) { cloned->addParentMixin(parent); });
        return cloned;
    }
}