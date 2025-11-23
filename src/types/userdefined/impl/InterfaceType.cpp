#include <unordered_set>
#include <utility>
#include "tmpl/CloneType.hpp"
#include "utils/TypeUtils.hpp"
#include "userdefined/ClassType.hpp"
#include "userdefined/InterfaceType.hpp"
#include <constants/LinkageType.hpp>
#include "resolver/methods/InterfaceMethodResolver.hpp"

namespace LynxTypes {

    // NOLINTNEXTLINE(misc-no-recursion)
    llvm::Type* InterfaceType::computeLLVMType() const {

        using namespace TypeUtils;
        
        if (cachedType) return cachedType;

        auto& llvmContext = astContext->getLLVMContext();

        // Step 1: get or create opaque struct
        const auto ifaceName = qualifiedName();
        auto* structType = getOrCreateStruct(llvmContext, ifaceName);
        registerLLVMType(structType);
        cachedType = structType;

        std::vector<llvm::Type*> vtableEntries;
        for (const auto* parent : parentInterfaces) {
            auto* parentVtable = llvm::cast<llvm::StructType>(parent->computeLLVMType());
            for (auto* elementType : parentVtable->elements()) {
                vtableEntries.push_back(elementType);
            }
        }

        for (const auto& [name, method] : methods) {
            std::vector<llvm::Type*> paramTypes;
            for (const auto& param : method->getParameterTypes()) {
                paramTypes.push_back(param->getLLVMType());
            }
            auto* retType = method->getReturnType()->getLLVMType();
            auto* funcType = llvm::FunctionType::get(retType, paramTypes, false);
            vtableEntries.push_back(llvm::PointerType::get(funcType->getContext(), 0));
        }

        setStructBodySafe(structType, vtableEntries);
        return cachedType;
    }

    llvm::Type* InterfaceType::getLLVMPointerType() const {
        if (!cachedPointerType) cachedPointerType = llvm::PointerType::get(getLLVMType()->getContext(), 0);
        return cachedPointerType;
    }

    llvm::Value* InterfaceType::getDefaultValue() {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(getLLVMPointerType()));
    }

    llvm::Value* InterfaceType::createInstance(const std::string& variableName) {
        auto& builder = astContext->getBuilder();
        auto* llvmType = computeLLVMType();
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
        if (cachedFullName.empty())  cachedFullName = "class." + interfaceName;
        return cachedFullName;            
    }

    const std::string& InterfaceType::originalNameLower() const {
        if (cachedLowerName.empty()) {
            cachedLowerName.reserve(interfaceName.size());
            for (const unsigned char name : interfaceName) {
                cachedLowerName += static_cast<char>(std::tolower(name));
            }
        }
        return cachedLowerName;
    }

    void InterfaceType::addParentInterface(const InterfaceType* iface) {
        for (const auto* existing : parentInterfaces) if (existing == iface) return;
        parentInterfaces.push_back(iface);        
    }

    void InterfaceType::addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method) {
        if (!methods.contains(mangleName)) return;
        methods[mangleName] = std::move(method);
    }

    const MethodType* InterfaceType::getMethod(const std::string& mangleName) const {
        auto itr = methods.find(mangleName);
        if (itr != methods.end())  return itr->second.get();
        return nullptr;
    }

    bool InterfaceType::hasMethod(const std::string& mangleName) const {
        return methods.contains(mangleName);
    }

    void InterfaceType::addField(const std::string& name, std::unique_ptr<FieldType> field) {
        if (!fields.contains(name)) return;
        fields[name] = std::move(field);
    }

    const FieldType* InterfaceType::getField(const std::string& name) const {
        auto itr = fields.find(name);
        if (itr != fields.end()) return itr->second.get();
        return nullptr;
    }

    std::vector<std::string> InterfaceType::getMethodOrder() const {
        std::unordered_set<const InterfaceType*> visited;
        std::vector<std::string> ordered;
        ordered.reserve(methods.size() + (parentInterfaces.size() * 4));
    
        std::function<void(const InterfaceType*)> collect = [&](const InterfaceType* iface) {
            if (!visited.insert(iface).second) return;

            for (const auto& parent : iface->parentInterfaces) collect(parent);

            for (const auto& [methodName, methodValue] : iface->methods) {
                auto itr = std::ranges::find(ordered, methodName);
                if (itr != ordered.end()) {
                    *itr = methodName;
                } else {
                    ordered.push_back(methodName);
                }
            }
        };

        collect(this);
        return ordered;
    }
    
    bool InterfaceType::hasField(const std::string& name) const {
        return fields.contains(name);
    }
        
    llvm::Value* InterfaceType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool InterfaceType::equals(const BaseType* other) const {
        if (const auto* iface = dynamic_cast<const InterfaceType*>(other)) {
            return interfaceName == iface->interfaceName;
        }
        return false;
    }

    void InterfaceType::registerLLVMType(llvm::StructType* structType) const {
        if (!structType) return;
        llvmTypeToClass[structType] = this;
    }

    const InterfaceType* InterfaceType::fromLLVMType(const llvm::Type* type) {
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

    void InterfaceType::buildVTable(const VTableType& vType) {
        auto [name, gvtable, structType, mapIdx] = vType;
        vtableName = name;
        vtableGlobal = gvtable; 
        vtableType = structType; 
        methodNameToIndex = mapIdx;
    }

    unsigned InterfaceType::getVirtualMethodIndex(const std::string& methodName) const {
        auto itr = methodNameToIndex.find(methodName);
        if (itr == methodNameToIndex.end()) throw std::runtime_error("Method not found in vtable: " + methodName);
        return itr->second;      
    }

    unsigned InterfaceType::methodIndex(const std::string& methodName) const {
        auto itr = methodNameToIndex.find(methodName);
        if (itr == methodNameToIndex.end()) throw std::runtime_error("Method not found in vtable: " + methodName);
        return itr->second;
    }

    llvm::Value* InterfaceType::getVTablePtrPtr(llvm::Value* objValue) const {
        auto itr = vtableCache.find(objValue);
        if (itr != vtableCache.end()) return itr->second;

        auto* type = computeLLVMType();
        auto* structType = llvm::dyn_cast<llvm::StructType>(type);
        auto& builder = astContext->getBuilder();
        auto* vtablePtrPtr = builder.CreateStructGEP(structType, objValue, 0, llvm::Twine(originalNameLower() + "_vtable_ptr_ptr"));   
        vtableCache[objValue] = vtablePtrPtr;
        return vtablePtrPtr;    
    }

    llvm::Value* InterfaceType::loadVTablePtr(llvm::Value* objValue) const {
        auto itr = vtableLoadCache.find(objValue);
        if (itr != vtableLoadCache.end()) return itr->second;
    
        auto* vtablePtrPtr = getVTablePtrPtr(objValue);
        auto* vtablePtrType = llvm::PointerType::get(vtableType->getContext(), 0); 

        auto& builder = astContext->getBuilder();
        auto* vtablePtr = builder.CreateLoad(vtablePtrType, vtablePtrPtr, llvm::Twine(originalNameLower() + + "_vtable"));
        vtableLoadCache[objValue] = vtablePtr;

        return vtablePtr;
    }

    llvm::Value* InterfaceType::loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const {
        auto& builder = astContext->getBuilder();
        const unsigned methodIndex = getVirtualMethodIndex(fnName);
        auto* methodPtrPtr = builder.CreateStructGEP(vtableType, vtablePtr, methodIndex, llvm::Twine(fnName + "_ptr_ptr"));
        auto* methodPtrType = llvm::PointerType::get(methodPtrPtr->getContext(), 0);
        return builder.CreateLoad(methodPtrType, methodPtrPtr, llvm::Twine(fnName + "_ptr"));
    }

    llvm::GlobalVariable* InterfaceType::getOrCreateOrVTableGlobal() const {
        auto* module = astContext->getModule();
        if (auto* existingGV = module->getGlobalVariable(vtableName, true)) return existingGV;
        if (!vtableGlobal) return nullptr;
        auto* newGV = new llvm::GlobalVariable(*module, vtableType, true, extType, nullptr, vtableName);
        return newGV;
    }

    std::string InterfaceType::resolveMethodCall(MethodKind /*kind*/, const std::string& mangledName, const std::vector<llvm::Type*>& /*args*/) const { return mangledName; }

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
        cloneMapContainer(fields, [&cloned](const auto& name, auto&& field) { cloned->addField(name, std::forward<decltype(field)>(field)); });
        cloneMapContainer(methods, [&cloned](const auto& name, auto&& method) { cloned->addMethod(name, std::forward<decltype(method)>(method)); });
        cloneVectorShallow(parentInterfaces, [&cloned](auto* parent) { cloned->addParentInterface(parent); });
        return cloned;
    }
}
