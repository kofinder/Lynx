
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "userdefined/ClassType.hpp"
#include "userdefined/InterfaceType.hpp"
#include <constants/LinkageType.hpp>
#include "resolver/methods/InterfaceMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* InterfaceType::computeLLVMType() const {

        using namespace TypeUtils;
        
        if (cachedType) return cachedType;

        std::string ifaceName = qualifiedName();
        auto& llvmContext = astContext->getLLVMContext();

        // Step 1: get or create opaque struct
        auto* structType = getOrCreateStruct(llvmContext, ifaceName);
        const_cast<InterfaceType*>(this)->registerLLVMType(structType);
        cachedType = structType;

        std::vector<llvm::Type*> vtableEntries;
        for (const auto* parent : parentInterfaces) {
            llvm::StructType* parentVtable = llvm::cast<llvm::StructType>(parent->computeLLVMType());
            for (auto* elementType : parentVtable->elements()) {
                vtableEntries.push_back(elementType);
            }
        }

        for (const auto& [name, method] : methods) {
            std::vector<llvm::Type*> paramTypes;
            for (auto& p : method->getParameterTypes()) {
                paramTypes.push_back(p->getLLVMType());
            }
            llvm::Type* retType = method->getReturnType()->getLLVMType();
            auto funcType = llvm::FunctionType::get(retType, paramTypes, false);
            vtableEntries.push_back(llvm::PointerType::get(funcType->getContext(), 0));
        }

        setStructBodySafe(structType, vtableEntries);
        return cachedType;
    }

    llvm::Type* InterfaceType::getLLVMPointerType() const {
        if (!cachedPointerType) {
            cachedPointerType = llvm::PointerType::get(getLLVMType()->getContext(), 0);
        } 
        return cachedPointerType;
    }

    llvm::Value* InterfaceType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(getLLVMPointerType()));
    }

    llvm::Value* InterfaceType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        auto llvmType = computeLLVMType();
        auto* var = builder.CreateAlloca(llvmType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), interfaceType));
            var->setMetadata(lynxDataType, metadata);
        }
        
        return var;
    }

    bool InterfaceType::isAssignable(BaseType* lhsType, BaseType* rhsType) {
        if (lhsType == rhsType) return true;
        if (auto* lhsIface = dynamic_cast<InterfaceType*>(lhsType)) {
            if (auto* rhsClass = dynamic_cast<ClassType*>(rhsType)) {
                return rhsClass->hasImplements(lhsIface);
            }
        }
        return false;    
    }

    const std::string& InterfaceType::qualifiedName() const { 
        if (cachedFullName.empty()) {
            cachedFullName = "class." + interfaceName;
        }
        return cachedFullName;            
    }

    const std::string& InterfaceType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(interfaceName.size());
            for (char c : interfaceName) {
                cachedLowerName += std::tolower(static_cast<unsigned char>(c));
            }
        }
        return cachedLowerName;    
    }

    void InterfaceType::addParentInterface(const InterfaceType* iface) {
        // Optional: check for duplicates before adding
        for (const auto* existing : parentInterfaces) {
            if (existing == iface) return;
        }
        parentInterfaces.push_back(iface);        
    }

    void InterfaceType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (methods.find(mangleName) != methods.end()) {
            std::cerr << "Warning: Method '" << mangleName << "' already exists in interface '" << interfaceName << "'\n";
            return;
        }
        methods[mangleName] = std::move(method);
    }

    const MethodType* InterfaceType::getMethod(const std::string& mangleName) const {
        auto it = methods.find(mangleName);
        if (it != methods.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool InterfaceType::hasMethod(const std::string& mangleName) const {
        return methods.find(mangleName) != methods.end();
    }

    void InterfaceType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (fields.find(name) != fields.end()) {
            std::cerr << "Warning: Field '" << name << "' already exists in interface '" << interfaceName << "'\n";
            return;
        }
        fields[name] = std::move(field);
    }

    const FieldType* InterfaceType::getField(const std::string& name) const {
        auto it = fields.find(name);
        if (it != fields.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    std::vector<std::string> InterfaceType::getMethodOrder() const {
        std::vector<std::string> ordered;
    
        // 1. Collect methods from parents first (left to right)
        for (const auto& parent : parentInterfaces) {
            auto parentOrder = parent->getMethodOrder();
            ordered.insert(ordered.end(), parentOrder.begin(), parentOrder.end());
        }
    
        // 2. Add own methods (override duplicates)
        for (const auto& [methodName, _] : methods) {
            // Replace if already exists (overriding)
            auto it = std::find(ordered.begin(), ordered.end(), methodName);
            if (it != ordered.end()) {
                *it = methodName;
            } else {
                ordered.push_back(methodName);
            }
        }
        
        return ordered;
    }    

    bool InterfaceType::hasField(const std::string& name) const {
        return fields.find(name) != fields.end();
    }
        
    llvm::Value* InterfaceType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool InterfaceType::equals(const BaseType* other) const {
        if (auto* iface = dynamic_cast<const InterfaceType*>(other)) {
            return this->interfaceName == iface->interfaceName;
        }
        return false;
    }

    void InterfaceType::registerLLVMType(llvm::StructType* structType) {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    InterfaceType* InterfaceType::fromLLVMType(const llvm::Type* type) {
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

    void InterfaceType::buildVTable(VTableType vType) {
        auto [name, gvtable, structType, mapIdx] = vType;
        vtableName = name;
        vtableGlobal = gvtable; 
        vtableType = structType; 
        methodNameToIndex = mapIdx;
    }

    unsigned InterfaceType::getVirtualMethodIndex(const std::string& methodName) const {
        auto it = methodNameToIndex.find(methodName);
        if (it == methodNameToIndex.end()) {
            LOG_ERROR("Method not found in vtable: {}", methodName);
            throw std::runtime_error("Method not found in vtable: " + methodName);
        }
        return it->second;      
    }

    unsigned InterfaceType::methodIndex(const std::string& methodName) const {
        auto it = methodNameToIndex.find(methodName);
        if (it == methodNameToIndex.end()) {
            LOG_ERROR("Method not found in vtable: {}", methodName);
            throw std::runtime_error("Method not found in vtable: " + methodName);
        }
        return it->second;
    }

    llvm::Value* InterfaceType::getVTablePtrPtr(llvm::Value* objValue) const {
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

    llvm::Value* InterfaceType::loadVTablePtr(llvm::Value* objValue) const {
        assert(objValue && "objValue cannot be null");

        auto it = vtableLoadCache.find(objValue);
        if (it != vtableLoadCache.end()) return it->second;
    
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);
        auto* vtablePtrType = llvm::PointerType::get(vtableType->getContext(), 0); 

        auto* vtablePtr = builder.CreateLoad(vtablePtrType, vtablePtrPtr, llvm::Twine(originalNameLower() + + "_vtable"));
        vtableLoadCache[objValue] = vtablePtr;

        return vtablePtr;
    }

    llvm::Value* InterfaceType::loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const {
        assert(vtablePtr && "vtablePtr cannot be null");
        auto& builder = astContext->getBuilder();
        unsigned methodIndex = getVirtualMethodIndex(fnName);
        auto* methodPtrPtr = builder.CreateStructGEP(vtableType, vtablePtr, methodIndex, llvm::Twine(fnName + "_ptr_ptr"));

        auto* methodPtrType = llvm::PointerType::get(methodPtrPtr->getContext(), 0);
        return builder.CreateLoad(methodPtrType, methodPtrPtr, llvm::Twine(fnName + "_ptr"));
    }

    llvm::GlobalVariable* InterfaceType::getOrCreateOrVTableGlobal() const {
        auto* module = astContext->getModule();
        if (auto* existingGV = module->getGlobalVariable(vtableName, true)) return existingGV;
    
        if (!vtableGlobal) {
            LOG_ERROR("VTable type not initialized for class: {}", interfaceName);
            return nullptr;
        }
        // auto* gvType = vtableGlobal->getValueType();
        auto* newGV = new llvm::GlobalVariable(*module, vtableType, true, extType, nullptr, vtableName);
        return newGV;
    }

    std::string InterfaceType::resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const {
        return mangledName;
    }

    const BaseType* InterfaceType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* InterfaceType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* InterfaceType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t InterfaceType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t InterfaceType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags InterfaceType::getDIFlags() const { return llvm::DINode::FlagZero; }

    std::unique_ptr<BaseType> LynxTypes::InterfaceType::clone() const {
        using namespace Cloned;
        auto cloned = std::make_unique<InterfaceType>(astContext, interfaceName);
        cloned->vtableType = vtableType;
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->addField(name, std::move(field)); });
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->addMethod(name, std::move(method)); });
        cloneVectorShallow(parentInterfaces, [&cloned](auto* parent) { cloned->addParentInterface(parent); });

        return cloned;
    }
}
