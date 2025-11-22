/**
 * @file InterfaceType.hpp
 * @brief Defines the InterfaceType class representing user-defined interfaces in the Lynx type system.
 *
 * The `InterfaceType` encapsulates user-defined interfaces that declare methods and fields
 * without providing concrete implementations. Interfaces support multiple inheritance and
 * enable polymorphic behavior via virtual method tables (vtables) in the Lynx language.
 *
 * **Key Responsibilities:**
 * - Represents user-defined interfaces that can be implemented by other types.
 * - Tracks methods and fields declared in the interface.
 * - Supports inheritance from multiple parent interfaces.
 * - Generates LLVM IR struct and pointer types for interface instances.
 * - Maintains vtables for virtual method resolution and polymorphic dispatch.
 * - Integrates with `TypeVisitor` for semantic checks and `MethodType` resolution.
 * - Produces DWARF-compatible debug information for interface types.
 *
 * **Integration Points:**
 * - Used in polymorphism, virtual method dispatch, and interface type assignments.
 * - Supports method resolution and super/interface calls.
 * - Provides lookup tables for fields, methods, and vtables for efficient LLVM code generation.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` with optional pointer types.
 * - Provides cached LLVM type and pointer type for performance.
 * - Maintains a single global LLVM vtable per interface containing all virtual methods.
 * - Supports loading virtual method pointers and vtable entries at runtime.
 *
 * **Additional Features:**
 * - Maintains method-to-index mapping for vtable layout.
 * - Supports registration and retrieval of LLVM types to `InterfaceType` mapping.
 * - Provides utility methods to check for fields, methods, and parent interfaces.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_INTERFACE_TYPE_HPP
#define LYNX_INTERFACE_TYPE_HPP

#include "helper/FieldType.hpp"
#include "helper/MethodType.hpp"
#include "helper/VTableType.hpp"
#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class InterfaceType : public UserDefinedType {

        private:

            std::string interfaceName;

            mutable llvm::StructType* cachedType = nullptr;

            mutable llvm::PointerType* cachedPointerType = nullptr;

            mutable std::string cachedFullName;

            mutable std::string cachedLowerName;

            std::vector<const InterfaceType*> parentInterfaces;

            std::unordered_map<std::string, std::unique_ptr<MethodType>> methods;

            std::unordered_map<std::string, std::unique_ptr<FieldType>> fields;

            mutable std::unordered_map<std::string, unsigned> fieldNameToIndex;

            /// @brief Global LLVM vtable for this class.
            /// 
            /// This global variable represents the vtable for the class and is
            /// created only **once per class**. It contains function pointers
            /// for all virtual methods and interface implementations. Each
            /// object of this class stores a pointer to this global vtable in
            /// its first field. **Do not duplicate this global per object.**
            llvm::GlobalVariable* vtableGlobal = nullptr;

            /// @brief LLVM struct type representing the vtable layout.
            llvm::StructType* vtableType = nullptr;

            /// @brief Name of the vtable in LLVM IR.
            std::string vtableName;

            /// @brief Map from method name to its index in the vtable.
            std::unordered_map<std::string, unsigned> methodNameToIndex;

            mutable std::unordered_map<llvm::Value*, llvm::Value*> vtableCache;
            mutable std::unordered_map<llvm::Value*, llvm::Value*> vtableLoadCache;

            static inline std::unordered_map<const llvm::StructType*, const InterfaceType*> llvmTypeToClass;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit InterfaceType(
                AstContext* context, 
                std::string interfaceName
            ) : UserDefinedType(context), interfaceName(std::move(interfaceName))  {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            std::unique_ptr<BaseType> clone() const override;
            
            bool equals(const BaseType* other) const override;

            bool isAssignable(BaseType* lhsType, BaseType* rhsType);

            void addParentInterface(const InterfaceType* iface);

            DataType getTypeTag() const override { return DataType::INTERFACE; }

            const std::string& qualifiedName() const;
            const std::string& originalNameLower() const;
            const std::string& originalName() const { return interfaceName; }

            void registerLLVMType(llvm::StructType* structTy) const;
            static const InterfaceType* fromLLVMType(const llvm::Type* type);

            bool hasMethod(const std::string& mangleName) const;
            void addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method);
            const MethodType* getMethod(const std::string& mangleName) const;
            std::vector<std::string> getMethodOrder() const;
            const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getMethods() const { return methods; }

            bool hasField(const std::string& name) const;
            void addField(const std::string& name, std::unique_ptr<FieldType> field);
            const FieldType* getField(const std::string& name) const;
            const std::unordered_map<std::string, unsigned>& getFieldNameToIndexMap() const { return fieldNameToIndex;} 
            const std::unordered_map<std::string, std::unique_ptr<FieldType>>& getFields() const { return fields; }
            const std::vector<const InterfaceType*>& getParents() const { return parentInterfaces; }

            void buildVTable(const VTableType& vType);
            llvm::GlobalVariable* getVTableGlobal() const;
            unsigned methodIndex(const std::string& name) const;
            llvm::GlobalVariable* getOrCreateOrVTableGlobal() const;
            llvm::Value* getVTablePtrPtr(llvm::Value* objValue) const;
            llvm::Value* loadVTablePtr(llvm::Value* objValue) const;
            llvm::Value* loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const;
            unsigned getVirtualMethodIndex(const std::string& methodName) const;
            std::string resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const;

            std::string getDebugName() const override { return "interface"; }
            llvm::DIType* getDIType(llvm::DIScope* scope) const override;
            uint64_t getDebugSizeInBits() const override;
            uint32_t getDebugAlignInBits() const override;
            llvm::DINode::DIFlags getDIFlags() const override;

            ~InterfaceType() override = default;
    };

}

#endif
