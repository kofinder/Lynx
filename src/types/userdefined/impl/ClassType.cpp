

#include <algorithm>
#include "tmpl/TypeCaster.hpp"
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "utils/DFSUtils.hpp"
#include "userdefined/ClassType.hpp"
#include <context/VirtualTable.hpp>
#include <context/AstContext.hpp>
#include <constants/LinkageType.hpp>
#include <context/GlobalSymbolContext.hpp>
#include <ast/tmpl/TypeConventionTemplate.hpp>
#include <ast/tmpl/ManglerTemplate.hpp>

namespace LynxTypes {

    using namespace LynxAst;
    using namespace TypeUtils;
    using namespace DFSUtils;

    llvm::Type* ClassType::computeLLVMType() const {

        if (cachedType) return cachedType;

        std::string name = qualifiedName();
        auto& context = astContext->getLLVMContext();

        // Step 1: get or create opaque struct
        auto* structType = getOrCreateStruct(context, name);
        cachedType = structType;

        // Step 2: Early cache pointer to this struct (for forward references)
        const_cast<ClassType*>(this)->registerLLVMType(structType);

        unsigned index = 0;
        fieldNameToIndex.clear();
        std::vector<llvm::Type*> members;

        // Step 3: vtable pointer if needed
        if (hasBaseClass() || !getAllVirtualParentMethods().empty()) {
            auto llvmPtrType = llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
            members.push_back(llvmPtrType); index++;
        }

        // 4. Base class members
        if (hasBaseClass()) {
            auto* structType = llvm::cast<llvm::StructType>(parentClass->computeLLVMType());
            for (auto* elem : structType->elements()) {
                members.push_back(elem); index++;
            }
        }

        // Step 4: Base interfaces
        if (hasInterfaces()) {
            for (const auto& iface : interfaces) {
                members.push_back(iface->getLLVMPointerType()); index++;
            }
        }

        // step 5. Base Mixin
        if(hasMixins()) {
            for(const auto& mixin: mixins) {
                members.push_back(mixin->getLLVMType()); index++;
            }
        }

        // Step 6: own fields
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

        setStructBodySafe(structType, members);

        return cachedType;
    }
    
    llvm::Type* ClassType::getLLVMPointerType() const {
        return llvm::PointerType::getUnqual(getLLVMType());
    }

    llvm::Value* ClassType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(getLLVMPointerType()));
    }

    llvm::Value* ClassType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* clazzType = this->getLLVMType();
        auto var = builder.CreateAlloca(clazzType, nullptr, variableName);
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

    const BaseType* ClassType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* ClassType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool ClassType::equals(const BaseType* other) const {
        if (auto* otherClass = dynamic_cast<const ClassType*>(other)) {
            return className== otherClass->className;
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

        // Direct implementation check
        for (const auto& up : interfaces) {
            if (up.get() == iface) {
                return true;
            }
        }

        // Inherited implementations via base classes
        const ClassType* cur = parentClass;
        while (cur) {
            // Access the vector from the parent; compare pointer identity
            for (const auto& up : cur->interfaces) {
                if (up.get() == iface) {
                    return true;
                }
            }
            cur = cur->parentClass;
        }

        return false;
    }

    const std::string& ClassType::qualifiedName() const { 
        if (cachedFullName.empty()) {
            cachedFullName = "class." + className;
        }
        return cachedFullName;            
    }

    const std::string& ClassType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(className.size());
            for (char c : className) {
                cachedLowerName += std::tolower(static_cast<unsigned char>(c));
            }
        }
        return cachedLowerName;    
    }


    std::string ClassType::getDebugName() const { return className; }

    llvm::DIType* ClassType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();
        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t ClassType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 64;
    }

    uint32_t ClassType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32;
    }

    llvm::DINode::DIFlags ClassType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }

    void ClassType::registerLLVMType(llvm::StructType* structType) {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    ClassType* ClassType::fromLLVMType(const llvm::Type* type) {
        if (!type) return nullptr;
        if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {
            type = ptrType->getPointerElementType();
        }

        if (auto structType = llvm::dyn_cast<llvm::StructType>(type)) {
            auto it = llvmTypeToClass.find(structType);
            if (it != llvmTypeToClass.end())  return it->second;
        }

        return nullptr;
    }

    void ClassType::addConstructor(const std::string& mangledName, std::unique_ptr<ConstructorType> ctor) {
        ctors[mangledName] = std::move(ctor);
    }

    const ConstructorType* ClassType::getConstructor(const std::string& mangledName) const {
        auto it = ctors.find(mangledName);
        if (it != ctors.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    std::string ClassType::resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const {

        if (kind == MethodKind::CONSTRUCTOR) {
            if (auto ctor = getConstructor(mangledName)) return mangledName;
        } else {
            if (auto method = getMethod(mangledName)) return mangledName;
        }

        auto viable = findViableCandidates(kind, argTypes);

        if (viable.empty()) throw std::runtime_error("No matching method found for class '" + className + "'");

        auto best = std::max_element(viable.begin(), viable.end(), [](const Candidate& a, const Candidate& b) {
            return a.score < b.score;
        });


        std::vector<llvm::Type*> arguments;
        arguments.clear();
        
        return best->mangled;
    }

    const std::vector<Candidate> ClassType::findViableCandidates(MethodKind kind, const std::vector<llvm::Type*>& argTypes) const {
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
            int64_t score = scoreParameters(params, argTypes);
            if (score >= 0) {
                viable.push_back({ mangledName, score });
            }
        }
        return viable;        
    }


    int64_t ClassType::scoreParameters(const std::vector<std::unique_ptr<BaseType>>& params, const std::vector<llvm::Type*>& argTypes) const {
        if (params.size() != argTypes.size()) return -1; // not viable

        int64_t score = 0;
        for (size_t i = 0; i < params.size(); ++i) {

            BaseType* expectedType = params[i].get();
            BaseType* actualType = convertLLVMTypeToBaseType(argTypes[i], *astContext);

            if (!expectedType || !actualType) return -1;

            // === 1. Exact match ===
            if (expectedType->equals(actualType)) { score += 10; continue; }

            // === 2. Implicit conversion ===
            if (expectedType->canAccept(actualType)) { score += 5; continue; }

            // === 3. Class → Base class conversion ===
            if (auto* expectedClass = TypeCasting::castType<const ClassType>(expectedType)) {
                if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
                    if (actualClass->isSubclassOf(expectedClass)) { score += 30; continue; }
                }
            }

            // === 4. Class → Interface conversion ===
            if (auto* expectedIface = TypeCasting::castType<const InterfaceType>(expectedType)) {
                if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
                    if (actualClass->implementsInterface(expectedIface)) { score += 30; continue; }
                }
            }

            // === 5. Class → Mixin conversion ===
            if (auto* expectedMixin = TypeCasting::castType<const MixinType>(expectedType)) {
                if (auto* actualClass = TypeCasting::castType<const ClassType>(actualType)) {
                    if (actualClass->usesMixin(expectedMixin)) { score += 30; continue; }
                }
            }
        }

        return score;
    }

    bool ClassType::hasMethod(const std::string& mangleName) const {
        return methods.find(mangleName) != methods.end();
    }

    void ClassType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (methods.find(mangleName) != methods.end()) {
            std::cerr << "Warning: Method '" << mangleName << "' already exists in interface '" << mangleName << "'\n";
            return;
        }
        methods[mangleName] = std::move(method);
    }
    
    const MethodType* ClassType::getMethod(const std::string& mangleName) const {
        auto it = methods.find(mangleName);
        if (it != methods.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool ClassType::hasField(const std::string& name) const {
        return fields.find(name) != fields.end();
    }

    void ClassType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (fields.find(name) != fields.end()) {
            std::cerr << "Warning: Field '" << name << "' already exists in interface '" << className << "'\n";
            return;
        }
        fields[name] = std::move(field);
    }

    const FieldType* ClassType::getField(const std::string& name) const {
        auto it = fields.find(name);
        if (it != fields.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    unsigned ClassType::getFieldIndex(const std::string& fieldName) const {
        auto it = fieldNameToIndex.find(fieldName);
        if (it == fieldNameToIndex.end()) {
            throw std::runtime_error("Field not found: " + fieldName);
        }
        return it->second;
    } 

    bool ClassType::hasImplements(const InterfaceType* iface) const {
        return std::any_of(
            interfaces.begin(),
            interfaces.end(),
            [iface](const std::unique_ptr<InterfaceType>& impl) { return impl.get() == iface; }
        );
    }

    bool ClassType::hasImplements(const std::string& ifaceName) const {
        return std::any_of(
            interfaces.begin(),
            interfaces.end(),
            [&ifaceName](const std::unique_ptr<InterfaceType>& impl) {
                return impl->originalName() == ifaceName;
            }
        );
    }

    void ClassType::addInterface(std::unique_ptr<InterfaceType> iface) {
        if (!iface) return;
        if (!hasImplements(iface->originalName())) {
            interfaces.push_back(std::move(iface)); // ✅ Move it
        }    
    }

    bool ClassType::usesMixin(const MixinType* mixin) const {
        return std::any_of(
            mixins.begin(),
            mixins.end(),
            [mixin](const std::unique_ptr<MixinType>& impl) { return impl.get() == mixin; }
        );
    }

    bool ClassType::usesMixin(const std::string& mixinName) const {
        return std::any_of(
            mixins.begin(),
            mixins.end(),
            [&mixinName](const std::unique_ptr<MixinType>& impl) {
                return impl->originalName() == mixinName;
            }
        );
    }

    void ClassType::addMixin(std::unique_ptr<MixinType> mixin) {
        if (!mixin) return;
        if (!usesMixin(mixin->originalName())) {
            mixins.push_back(std::move(mixin));
        }   
    }

    int ClassType::getMixinOffset(const MixinType& mixin) const {
        for (unsigned i = 0; i < mixins.size(); ++i) {
            if (mixins[i]->originalName() == mixin.originalName()) return i;
        }
        throw std::runtime_error("Mixin '" + mixin.originalName() + "' not found in class " + className);
    }

    const std::vector<MixinOwner> ClassType::getAllMixinMethods() {
    
        // 1. Topological sort over mixins + parent mixins
        std::unordered_map<const MixinType*, VisitState> state;
        std::vector<const MixinType*> orderedMixins;
    
        for (const auto& mixin : mixins) topoDFS(mixin.get(), state, orderedMixins);
        std::reverse(orderedMixins.begin(), orderedMixins.end());
    
        // 2. Flatten methods with left-to-right override resolution
        std::vector<MixinOwner> mixinOwners;
        for (const auto* mixin : orderedMixins) {
            auto it = std::find_if(
                mixinOwners.begin(),
                mixinOwners.end(),
                [&](const MixinOwner& owner) { return owner.mixinName == mixin->originalNameLower(); }
            );
            if (it != mixinOwners.end())  continue; // skip duplicate mixin

            MixinOwner owner;
            owner.mixinName = mixin->originalNameLower();
            owner.mixinOffset = getMixinOffset(*mixin);

            for (const auto& [name, methodPtr] : mixin->getMethods()) {
                owner.methodMap[name] = methodPtr.get();
            }

            if (!owner.methodMap.empty()) mixinOwners.push_back(std::move(owner));
        }

       // 3. Keep only the *final* owners for each method
        std::unordered_map<std::string, bool> seen;
        std::vector<MixinOwner> finalOwners;
        std::vector<const MethodType*> resolved;
        for(const auto owner: mixinOwners) {
            MixinOwner filtered;
            filtered.mixinName = owner.mixinName;
            filtered.mixinOffset = owner.mixinOffset;
            for (const auto& [name, method] : owner.methodMap) {
                auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, method->getName(), method->getParameterRawTypes());
                if (seen.insert({mangledName, true}).second) {
                    filtered.methodMap[name] = method;
                    resolved.push_back(method);
                }
                if (!filtered.methodMap.empty()) finalOwners.push_back(std::move(filtered));
            }
        }

        // 4. Inject mixn methods into the class
        for (const auto& method : resolved) {
            auto mangleName = Mangle::get(ManglerKind::MEMBER_FUNCTION, className, method->getName(), method->getParameterRawTypes());
            addMethod(mangleName, method->clone()); // copy method
        }
        
        std::reverse(finalOwners.begin(), finalOwners.end());
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

    const std::vector<std::string> ClassType::getAllVirtualParentMethodsNames() const {
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
        auto it = methods.find(name);
        if (it == methods.end()) return false;
        return it->second->isVirtual();    
    }

    unsigned ClassType::getVirtualMethodIndex(const std::string& methodName) const {
        auto it = methodNameToIndex.find(methodName);
        if (it == methodNameToIndex.end()) {
            throw std::runtime_error("Method not found in vtable: " + methodName);
        }
        return it->second;      
    }

    void ClassType::buildVTable(VTableType vType) { 
        auto [name, gvtable, structType, mapIdx] = vType;
        vtableName = name;
        vtableGlobal = gvtable; 
        vtableType = structType; 
        methodNameToIndex = mapIdx;
    }

    void ClassType::bindVTable(llvm::Value* objValue) {
        assert(objValue && "objValue cannot be null");

        auto* vGlobal = getOrCreateOrVTableGlobal();
        if (!vGlobal) {
            LOG_ERROR("VTable global could not be created for class: {}", vtableName);
            return;
        }
    
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);
        builder.CreateStore(vGlobal, vtablePtrPtr); 
    }

    llvm::Value* ClassType::getVTablePtrPtr(llvm::Value* objValue) const {
        assert(objValue && "objValue cannot be null");

        auto it = vtableCache.find(objValue);
        if (it != vtableCache.end()) return it->second;

        auto& builder = astContext->getBuilder();
        auto* type = computeLLVMType();
        auto* structType = llvm::dyn_cast<llvm::StructType>(type);
        assert(structType && "Expected a StructType");
        assert(structType->getNumElements() > 0 && "Struct type must have at least one element for vtable");
    
        auto* vtablePtrPtr = builder.CreateStructGEP(structType, objValue, 0, llvm::Twine(originalNameLower() + "_vtable_ptr_ptr"));   
        
        vtableCache[objValue] = vtablePtrPtr;
        return vtablePtrPtr;    
    }

    llvm::Value* ClassType::loadVTablePtr(llvm::Value* objValue) const {
        assert(objValue && "objValue cannot be null");

        auto it = vtableLoadCache.find(objValue);
        if (it != vtableLoadCache.end()) return it->second;
    
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);

        auto* vtablePtr = builder.CreateLoad(vtableType->getPointerTo(), vtablePtrPtr, llvm::Twine(originalNameLower() + + "_vtable"));
        vtableLoadCache[objValue] = vtablePtr;

        return vtablePtr;
    }

    llvm::Value* ClassType::loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const {
        assert(vtablePtr && "vtablePtr cannot be null");
        auto& builder = astContext->getBuilder();
        unsigned methodIndex = getVirtualMethodIndex(fnName);
        auto* methodPtrPtr = builder.CreateStructGEP(vtableType, vtablePtr, methodIndex, llvm::Twine(fnName + "_ptr_ptr"));
        return builder.CreateLoad(methodPtrPtr->getType()->getPointerElementType(), methodPtrPtr, llvm::Twine(fnName + "_ptr"));
    }

    llvm::GlobalVariable* ClassType::getOrCreateOrVTableGlobal() const {
        std::cout << "[class] ===>" << className << " [vtableName]  ===>" << vtableName << std::endl;

        auto* module = astContext->getModule();
        if (auto* existingGV = module->getGlobalVariable(vtableName, true)) return existingGV;
    
        if (!vtableGlobal) {
            LOG_ERROR("VTable type not initialized for class: {}", className);
            return nullptr;
        }
        
        if (vtableName.empty()) {
            LOG_ERROR("VTable '{}' does not declare virtual methods; skipping vtable", className);
            return nullptr;
        }    

        // llvm::errs() << "vtableType = ";
        // vtableType->print(llvm::errs());
        // llvm::errs() << "\n";
        // llvm::errs() << "cached vtableGlobal->getValueType() = ";
        // vtableGlobal->getValueType()->print(llvm::errs()); llvm::errs() << "\n";
        // auto* gvType = vtableGlobal->getValueType();

        auto* newGV = new llvm::GlobalVariable(*module, vtableType, true, extType, nullptr, vtableName);
        return newGV;
    }

    std::unique_ptr<BaseType> ClassType::clone() const {
        using namespace Cloned;
        auto cloned = std::make_unique<ClassType>(astContext, className);
        cloned->parentClass = parentClass;
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->methods[name] = std::move(method);});
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->fields[name] = std::move(field); });
        cloneVectorContainer(interfaces, [&cloned](auto&& iface) { cloned->interfaces.push_back(std::move(iface)); });
        return cloned;
    }
}
