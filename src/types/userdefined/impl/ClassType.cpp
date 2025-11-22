#include <algorithm>
#include "tmpl/TypeCaster.hpp"
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "utils/DFSUtils.hpp"
#include "userdefined/ClassType.hpp"
#include "helper/TypeScoring.hpp"
#include <context/VirtualTable.hpp>
#include <context/AstContext.hpp>
#include <constants/LinkageType.hpp>
#include <context/GlobalSymbolContext.hpp>
#include <ast/tmpl/TypeConventionTemplate.hpp>
#include <ast/tmpl/ManglerTemplate.hpp>
#include "resolver/methods/ClassMethodResolver.hpp"

namespace LynxTypes {

    using namespace LynxAst;
    using namespace TypeUtils;
    using namespace DFSUtils;

    // NOLINTNEXTLINE(misc-no-recursion)
    llvm::Type* ClassType::computeLLVMType() const {

        if (cachedType) return cachedType;
    
        auto& context = astContext->getLLVMContext();
    
        // Step 1: get or create the Opaque struct
        const auto name = qualifiedName();
        auto* structType = getOrCreateStruct(context, name);
        cachedType = structType;
    
        // Step 2: register early for forward references (not a const-cast anymore)
        registerLLVMType(structType);
    
        std::size_t index = 0;
        fieldNameToIndex.clear();
        std::vector<llvm::Type*> members;
    
        // Step 3: VTable pointer
        if (hasBaseClass() || !getAllVirtualParentMethods().empty()) {
            auto* i8Ty = llvm::Type::getInt8Ty(context);
            auto* i8PtrTy = llvm::PointerType::get(i8Ty->getContext(), 0);
            auto* vtablePtrTy = llvm::PointerType::get(i8PtrTy->getContext(), 0);
            members.push_back(vtablePtrTy);
            ++index;
        }
    
        // Step 4: Base class members
        if (hasBaseClass()) {
            auto* parentStructType =
                llvm::cast<llvm::StructType>(parentClass->computeLLVMType());
            for (auto* elem : parentStructType->elements()) {
                members.push_back(elem);
                ++index;
            }
        }
    
        // Step 5: Interfaces
        for (const auto& iface : interfaces) {
            members.push_back(iface->getLLVMPointerType());
            ++index;
        }
    
        // Step 6: Mixins
        for (const auto& mixin : mixins) {
            members.push_back(mixin->getLLVMType());
            ++index;
        }
    
        // Step 7: Own fields
        for (const auto& fieldEntry : fields) {
            const auto& fieldName = fieldEntry.first;
            const auto& field = fieldEntry.second;
    
            const BaseType* fieldType = field->getType();
            if (!fieldType) continue;
    
            llvm::Type* fieldLLVMType = nullptr; // NOLINT(misc-const-correctness)
            if (const auto* dateField = TypeCasting::castType<DateTimeType>(fieldType)) {
                fieldLLVMType = dateField->getLLVMPointerType();
            } else if (const auto* fileField = TypeCasting::castType<FileType>(fieldType)) {
                fieldLLVMType = fileField->getLLVMPointerType();
            } else if (const auto* classField = TypeCasting::castType<ClassType>(fieldType)) {
                fieldLLVMType = classField->getLLVMPointerType();
            } else if (const auto* ifaceField = TypeCasting::castType<InterfaceType>(fieldType)) {
                fieldLLVMType = ifaceField->getLLVMPointerType();
            } else {
                fieldLLVMType = field->getType()->getLLVMType();
            }
    
            members.push_back(fieldLLVMType);
            fieldNameToIndex[fieldName] = index++;
        }
    
        setStructBodySafe(structType, members);
    
        return cachedType;
    }
    
    llvm::Type* ClassType::getLLVMPointerType() const {
        return llvm::PointerType::get(getLLVMType()->getContext(), 0);
    }

    llvm::Value* ClassType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(getLLVMPointerType()));
    }

    llvm::Value* ClassType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* clazzType = this->getLLVMType();
        auto* var = builder.CreateAlloca(clazzType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), classType));
            var->setMetadata(lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* ClassType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool ClassType::equals(const BaseType* other) const {
        if (const auto* otherClass = dynamic_cast<const ClassType*>(other)) {
            return className == otherClass->className;
        }
        return false;
    }

    bool ClassType::isSubclassOf(const ClassType* base) const {
        if (base == nullptr) return false;
        if (this == base) return true;

        const ClassType* cur = parentClass;
        while (cur) {
            if (cur == base) return true;
            cur = cur->parentClass;
        }
        return false;
    }

    bool ClassType::implementsInterface(const InterfaceType* iface) const {
        if (iface == nullptr) return false;

        for (const auto& clz : interfaces) {
            if (clz.get() == iface) return true;
        }

        const ClassType* cur = parentClass;
        while (cur) {
            for (const auto& clf : cur->interfaces) {
                if (clf.get() == iface) return true;
            }
            cur = cur->parentClass;
        }

        return false;
    }

    const std::string& ClassType::qualifiedName() const { 
        if (cachedFullName.empty()) cachedFullName = "class." + className;
        return cachedFullName;            
    }

    const std::string& ClassType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(className.size());
            for (const unsigned char name : className) {
                cachedLowerName += static_cast<char>(std::tolower(name));
            }
        }
        return cachedLowerName;
    }    

    void ClassType::registerLLVMType(llvm::StructType* structType) const {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    const ClassType* ClassType::fromLLVMType(const llvm::Type* type) {
        if (!type) return nullptr;
        if (const auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            auto itr = llvmTypeToClass.find(structType);
            if (itr != llvmTypeToClass.end())  return itr->second;
        }
        return nullptr;
    }

    void ClassType::addConstructor(const std::string& mangledName, std::unique_ptr<ConstructorType> ctor) {
        ctors[mangledName] = std::move(ctor);
    }

    const ConstructorType* ClassType::getConstructor(const std::string& mangledName) const {
        auto itr = ctors.find(mangledName);
        if (itr != ctors.end()) return itr->second.get();
        return nullptr;
    }

    std::string ClassType::resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const {

        if (kind == MethodKind::CONSTRUCTOR) {
            if (const auto* ctor = getConstructor(mangledName)) return mangledName;
        } else {
            if (const auto* method = getMethod(mangledName)) return mangledName;
        }

        auto viable = findViableCandidates(kind, argTypes);

        if (viable.empty()) throw std::runtime_error("No matching method found for class '" + className + "'");

        const auto bestIterator = std::ranges::max_element(viable, {}, &Candidate::score);
    
        return bestIterator->mangled;
    }

    std::vector<Candidate> ClassType::findViableCandidates(MethodKind kind, const std::vector<llvm::Type*>& argTypes) const {
        return (kind == MethodKind::CONSTRUCTOR)
        ? findViableCandidatesImpl(ctors, argTypes)
        : findViableCandidatesImpl(methods, argTypes);
    }

    template <typename MapT>
    std::vector<Candidate> ClassType::findViableCandidatesImpl(const MapT& items, const std::vector<llvm::Type*>& argTypes) const {
        std::vector<Candidate> viable;
        viable.reserve(items.size());
        for (const auto& [mangledName, item] : items) {
            const auto& params = item->getParameterTypes();
            const int64_t score = scoreParameters(params, argTypes);
            if (score >= 0) viable.push_back({ mangledName, score });
        }
        return viable;        
    }

    int64_t ClassType::scoreParameters(
        const std::vector<std::unique_ptr<BaseType>>& params,
        const std::vector<llvm::Type*>& argTypes
    ) const {
        if (params.size() != argTypes.size()) return -1;

        int64_t totalScore = 0;
        for (size_t i = 0; i < params.size(); ++i) {
            BaseType* expected = params[i].get();
            const BaseType* actual = convertLLVMTypeToBaseType(argTypes[i], *astContext);
            const int64_t score = scoreSingleParameter(expected, actual);
            if (score < 0) return -1;
            totalScore += score;
        }

        return totalScore;
    }

    bool ClassType::hasMethod(const std::string& mangleName) const {
        return methods.contains(mangleName);
    }

    void ClassType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (!methods.contains(mangleName)) return;
        methods[mangleName] = std::move(method);
    }
    
    const MethodType* ClassType::getMethod(const std::string& mangleName) const {
        auto itr = methods.find(mangleName);
        if (itr != methods.end()) return itr->second.get();
        return nullptr;
    }

    bool ClassType::hasField(const std::string& name) const {
        return fields.contains(name);
    }

    void ClassType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (fields.contains(name)) return;
        fields[name] = std::move(field);
    }

    const FieldType* ClassType::getField(const std::string& name) const {
        auto itr = fields.find(name);
        if (itr != fields.end()) {
            return itr->second.get();
        }
        return nullptr;
    }

    unsigned ClassType::getFieldIndex(const std::string& fieldName) const {
        auto itr = fieldNameToIndex.find(fieldName);
        if (itr == fieldNameToIndex.end()) {
            throw std::runtime_error("Field not found: " + fieldName);
        }
        return itr->second;
    } 

    void ClassType::addInterface(std::unique_ptr<InterfaceType> iface) {
        if (!iface) return;
        if (!hasImplements(iface->originalName())) {
            interfaces.push_back(std::move(iface)); // ✅ Move itr
        }    
    }

    bool ClassType::hasImplements(const InterfaceType* iface) const {
        return std::ranges::any_of(interfaces, [iface](const auto& impl) {
            return impl.get() == iface;
        });
    }
    
    bool ClassType::hasImplements(const std::string& ifaceName) const {
        return std::ranges::any_of(interfaces, [&ifaceName](const auto& impl) {
            return impl->originalName() == ifaceName;
        });
    }
    
    bool ClassType::usesMixin(const MixinType* mixin) const {
        return std::ranges::any_of(mixins, [mixin](const auto& impl) {
            return impl.get() == mixin;
        });
    }
    
    bool ClassType::usesMixin(const std::string& mixinName) const {
        return std::ranges::any_of(mixins, [&mixinName](const auto& impl) {
            return impl->originalName() == mixinName;
        });
    }
    
    void ClassType::addMixin(std::unique_ptr<MixinType> mixin) {
        if (!mixin) return;
        if (!usesMixin(mixin->originalName())) {
            mixins.push_back(std::move(mixin));
        }   
    }

    int ClassType::getMixinOffset(const MixinType& mixin) const {
        for (int i = 0; i < mixins.size(); ++i) {
            if (mixins[i]->originalName() == mixin.originalName()) return i;
        }
        throw std::runtime_error("Mixin '" + mixin.originalName() + "' not found in class " + className);
    }

    std::vector<MixinOwner> ClassType::getAllMixinMethods() {
        // 1. Topological sort of mixins + all their parents
        std::unordered_map<const MixinType*, VisitState> visitState;
        std::vector<const MixinType*> orderedMixins;
    
        for (const auto& mixinPtr : mixins) {
            topoDFS(mixinPtr.get(), visitState, orderedMixins);
        }
    
        std::ranges::reverse(orderedMixins);
    
        // 2. Flatten immediate method owners (dedupe mixins by original name)
        std::vector<MixinOwner> mixinOwners;
        mixinOwners.reserve(orderedMixins.size());
    
        for (const auto* mixin : orderedMixins) {
            const auto mixinKey = mixin->originalNameLower();
    
            // skip duplicates
            if (std::ranges::any_of(mixinOwners, [&](const MixinOwner& owner) {
                return owner.mixinName == mixinKey;
            })) {
                continue;
            }
    
            MixinOwner owner;
            owner.mixinName = mixinKey;
            owner.mixinOffset = getMixinOffset(*mixin);
    
            for (const auto& [methodName, methodPtr] : mixin->getMethods()) {
                owner.methodMap[methodName] = methodPtr.get();
            }
    
            if (!owner.methodMap.empty()) {
                mixinOwners.push_back(std::move(owner));
            }
        }
    
        // 3. Keep only final owners for each unique method signature
        std::unordered_set<std::string> seenSignatures;
        std::vector<MixinOwner> finalOwners;
        std::vector<const MethodType*> resolved;
    
        for (const auto& owner : mixinOwners) {            // ← fixed: reference, no copy
            MixinOwner filtered;
            filtered.mixinName = owner.mixinName;
            filtered.mixinOffset = owner.mixinOffset;
    
            for (const auto& [methodName, method] : owner.methodMap) {
                const std::string signature =
                    Mangle::get(ManglerKind::MEMBER_FUNCTION,
                                method->getName(),
                                method->getParameterRawTypes());
    
                if (seenSignatures.insert(signature).second) {
                    filtered.methodMap[methodName] = method;
                    resolved.push_back(method);
                }
            }
    
            if (!filtered.methodMap.empty()) {
                finalOwners.push_back(std::move(filtered));
            }
        }
    
        // 4. Inject resolved mixin methods into the class
        for (const MethodType* method : resolved) {
            const std::string mangled =
                Mangle::get(ManglerKind::MEMBER_FUNCTION,
                            className,
                            method->getName(),
                            method->getParameterRawTypes());
    
            addMethod(mangled, method->clone());
        }
    
        std::ranges::reverse(finalOwners);
        return finalOwners;
    }
    

    std::vector<std::unique_ptr<MethodType>> ClassType::getAllVirtualParentMethods() const {
        std::vector<std::unique_ptr<MethodType>> result;
        const ClassType* current = parentClass;
        while (current) {
            for (const auto& [name, methodPtr] : current->getMethods()) {
                if (methodPtr->isVirtual()) {
                    result.push_back(methodPtr->clone());
                }
            }
            current = current->getBaseClass();
        }
        return result;
    }    

    std::vector<std::string> ClassType::getAllVirtualParentMethodsNames() const {
        std::vector<std::string> result;
        const ClassType* current = parentClass;
        while (current) {
            for (const auto& [name, methodPtr] : current->getMethods()) {
                if (methodPtr->isVirtual()) {
                    result.push_back(name);
                }
            }
            current = current->getBaseClass();
        }
        return result;
    }

    bool ClassType::isVirtualFunction(const std::string& name) const {
        auto itr = methods.find(name);
        if (itr == methods.end()) return false;
        return itr->second->isVirtual();    
    }

    unsigned ClassType::getVirtualMethodIndex(const std::string& methodName) const {
        auto itr = methodNameToIndex.find(methodName);
        if (itr == methodNameToIndex.end()) {
            throw std::runtime_error("Method not found in vtable: " + methodName);
        }
        return itr->second;      
    }

    void ClassType::buildVTable(const VTableType& vType) { 
        auto [name, gvtable, structType, mapIdx] = vType;
        vtableName = name;
        vtableGlobal = gvtable; 
        vtableType = structType; 
        methodNameToIndex = mapIdx;
    }

    void ClassType::bindVTable(llvm::Value* objValue) {
        auto* vGlobal = getOrCreateOrVTableGlobal();
        if (!vGlobal)  return;    
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);
        builder.CreateStore(vGlobal, vtablePtrPtr); 
    }

    llvm::Value* ClassType::getVTablePtrPtr(llvm::Value* objValue) const {
        auto itr = vtableCache.find(objValue);
        if (itr != vtableCache.end()) return itr->second;

        auto& builder = astContext->getBuilder();
        auto* type = computeLLVMType();
        auto* structType = llvm::dyn_cast<llvm::StructType>(type);    
        auto* vtablePtrPtr = builder.CreateStructGEP(structType, objValue, 0, llvm::Twine(originalNameLower() + "_vtable_ptr_ptr"));   
        
        vtableCache[objValue] = vtablePtrPtr;
        return vtablePtrPtr;    
    }

    llvm::Value* ClassType::loadVTablePtr(llvm::Value* objValue) const {
        auto itr = vtableLoadCache.find(objValue);
        if (itr != vtableLoadCache.end()) return itr->second;
    
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);
        auto* vtablePtrType = llvm::PointerType::get(vtableType->getContext(), 0); 

        auto* vtablePtr = builder.CreateLoad(vtablePtrType, vtablePtrPtr, llvm::Twine(originalNameLower() + + "_vtable"));
        vtableLoadCache[objValue] = vtablePtr;
        return vtablePtr;
    }

    llvm::Value* ClassType::loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const {
        auto& builder = astContext->getBuilder();
        const unsigned methodIndex = getVirtualMethodIndex(fnName);
        auto* methodPtrPtr = builder.CreateStructGEP(vtableType, vtablePtr, methodIndex, llvm::Twine(fnName + "_ptr_ptr"));
        auto* methodPrtTy = llvm::PointerType::get(methodPtrPtr->getContext(), 0);
        return builder.CreateLoad(methodPrtTy, methodPtrPtr, llvm::Twine(fnName + "_ptr"));
    }

    llvm::GlobalVariable* ClassType::getOrCreateOrVTableGlobal() const {
        auto* module = astContext->getModule();
        if (auto* existingGV = module->getGlobalVariable(vtableName, true)) return existingGV;
    
        if (!vtableGlobal) return nullptr;
        if (vtableName.empty()) return nullptr;

        // llvm::errs() << "vtableType = ";
        // vtableType->print(llvm::errs());
        // llvm::errs() << "\n";
        // llvm::errs() << "cached vtableGlobal->getValueType() = ";
        // vtableGlobal->getValueType()->print(llvm::errs()); llvm::errs() << "\n";
        // auto* gvType = vtableGlobal->getValueType();

        auto* newGV = new llvm::GlobalVariable(*module, vtableType, true, extType, nullptr, vtableName);
        return newGV;
    }
    
    const BaseType* ClassType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ClassType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ClassType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ClassType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ClassType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ClassType::getDIFlags() const { return llvm::DINode::FlagZero; }

    std::unique_ptr<BaseType> ClassType::clone() const {
        using namespace Cloned;
        auto cloned = std::make_unique<ClassType>(astContext, className);
        cloned->parentClass = parentClass;
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->methods[name] = std::forward<decltype(method)>(method); });
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->fields[name] = std::forward<decltype(field)>(field); });
        cloneVectorContainer(interfaces, [&cloned](auto&& iface) { cloned->interfaces.push_back(std::forward<decltype(iface)>(iface)); });
        return cloned;
    }
}




// int64_t ClassType::scoreParameters(const std::vector<std::unique_ptr<BaseType>>& params, const std::vector<llvm::Type*>& argTypes) const {
//     if (params.size() != argTypes.size()) return -1; // not viable

//     int64_t score = 0;
//     for (size_t i = 0; i < params.size(); ++i) {

//         BaseType* expectedType = params[i].get();
//         const auto* actualType = convertLLVMTypeToBaseType(argTypes[i], *astContext);

//         if (!expectedType || !actualType) return -1;

//         // === 1. Exact match ===
//         if (expectedType->equals(actualType)) { score += 10; continue; }

//         // === 2. Implicit conversion ===
//         if (expectedType->canAccept(actualType)) { score += 5; continue; }

//         // === 3. Class → Base class conversion ===
//         if (auto* expectedClass = TypeCasting::castType<const ClassType>(expectedType)) {
//             if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
//                 if (actualClass->isSubclassOf(expectedClass)) { score += 30; continue; }
//             }
//         }

//         // === 4. Class → Interface conversion ===
//         if (auto* expectedIface = TypeCasting::castType<const InterfaceType>(expectedType)) {
//             if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
//                 if (actualClass->implementsInterface(expectedIface)) { score += 30; continue; }
//             }
//         }

//         // === 5. Class → Mixin conversion ===
//         if (auto* expectedMixin = TypeCasting::castType<const MixinType>(expectedType)) {
//             if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
//                 if (actualClass->usesMixin(expectedMixin)) { score += 30; continue; }
//             }
//         }
//     }

//     return score;
// }
