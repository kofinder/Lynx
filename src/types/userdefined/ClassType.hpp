/**
 * @file ClassType.hpp
 * @brief Defines the ClassType class representing user-defined classes in the Lynx type system.
 *
 * The `ClassType` encapsulates user-defined classes that can inherit from a single base class,
 * implement multiple interfaces, and use mixins. It supports fields, methods, constructors,
 * virtual method dispatch, LLVM IR generation, and debug metadata.
 *
 * **Key Responsibilities:**
 * - Represents user-defined classes in the Lynx language.
 * - Supports single inheritance (base class) and multiple interface implementation.
 * - Supports mixins to extend functionality with fields and methods.
 * - Tracks methods, fields, and constructors, including virtual methods for polymorphism.
 * - Generates LLVM IR struct and pointer types for class instances.
 * - Maintains a global LLVM vtable for virtual method dispatch.
 * - Integrates with `TypeVisitor` for semantic analysis.
 * - Produces DWARF-compatible debug information for class types.
 *
 * **Integration Points:**
 * - Used for object instantiation, assignment, and polymorphic method calls.
 * - Supports virtual function dispatch via vtables.
 * - Interoperates with `MixinType` and `InterfaceType` for mixin and interface handling.
 * - Provides field and method lookup tables for LLVM code generation.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` with optional pointer types.
 * - Provides cached LLVM type and pointer type for performance.
 * - Maintains a single global LLVM vtable per class containing virtual method pointers.
 * - Supports binding vtable pointers in object instances.
 *
 * **Additional Features:**
 * - Provides utilities for checking inheritance, interface implementation, and mixin usage.
 * - Maintains method-to-index and field-to-index mappings for efficient LLVM layout.
 * - Preserves constructor, method, and field registration and lookup.
 * - Supports scoring and selecting viable method candidates for overload resolution.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_CLASS_TYPE_HPP
#define LYNX_CLASS_TYPE_HPP

#include "MixinType.hpp"
#include "InterfaceType.hpp"
#include "helper/ConstructorType.hpp"
#include <types/interfaces/BaseType.hpp>
#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    struct MixinOwner { 
        int mixinOffset; 
        std::string mixinName; 
        std::unordered_map<std::string, MethodType*> methodMap; 
    };

    class ClassType : public UserDefinedType {

        private:

            std::string className;

            ClassType* parentClass = nullptr; 

            mutable llvm::StructType* cachedType = nullptr;

            mutable std::string cachedFullName;

            mutable std::string cachedLowerName;

            std::vector<std::unique_ptr<InterfaceType>> interfaces;

            std::vector<std::unique_ptr<MixinType>> mixins;

            std::unordered_map<std::string, std::unique_ptr<MethodType>> methods;

            std::unordered_map<std::string, std::unique_ptr<FieldType>> fields;

            std::unordered_map<std::string, std::unique_ptr<ConstructorType>> ctors;

            mutable std::unordered_map<std::string, unsigned> fieldNameToIndex;

            std::unordered_map<std::string, unsigned> methodNameToIndex;

            /// @brief Global LLVM vtable for this class.
            /// 
            /// This global variable represents the vtable for the class and is
            /// created only **once per class**. It contains function pointers
            /// for all virtual methods and interface implementations. Each
            /// object of this class stores a pointer to this global vtable in
            /// its first field. **Do not duplicate this global per object.**
            std::string vtableName;
            llvm::StructType* vtableType = nullptr;
            llvm::GlobalVariable* vtableGlobal = nullptr;

            mutable std::unordered_map<llvm::Value*, llvm::Value*> vtableCache;
            mutable std::unordered_map<llvm::Value*, llvm::Value*> vtableLoadCache;
            static inline std::unordered_map<const llvm::StructType*, const ClassType*> llvmTypeToClass;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

            template <typename MapT>
            std::vector<Candidate> findViableCandidatesImpl(const MapT& items, const std::vector<llvm::Type*>& argTypes) const;

            std::vector<Candidate> findViableCandidates(MethodKind kind, const std::vector<llvm::Type*>& argTypes) const;

            int64_t scoreParameters(const std::vector<std::unique_ptr<BaseType>>& params, const std::vector<llvm::Type*>& argTypes) const;

        public:
        
            explicit ClassType(
                AstContext* context, 
                const std::string& name
            ) : UserDefinedType(context), className(name) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            std::unique_ptr<BaseType> clone() const override;

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::CLAZZ; }

            const std::string& qualifiedName() const;
            const std::string& originalNameLower() const;
            const std::string& originalName() const { return className; }

            void registerLLVMType(llvm::StructType* structTy) const;
            static const ClassType* fromLLVMType(const llvm::Type* type);

            // ---------------------
            // Constructor Handling
            // ---------------------
            void addConstructor(const std::string& mangledName, std::unique_ptr<ConstructorType> ctor);
            const ConstructorType* getConstructor(const std::string& mangledName) const;
            const std::unordered_map<std::string, std::unique_ptr<ConstructorType>>& getConstructors() const { return ctors; }
            
            // ---------------------
            // Method Handling
            // ---------------------
            bool hasMethod(const std::string& mangleName) const;
            const MethodType* getMethod(const std::string& mangleName) const;
            void addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method);
            const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getMethods() const { return methods; }

            // ---------------------
            // Fields Handling
            // ---------------------
            bool hasField(const std::string& name) const;
            void addField(const std::string& name, std::unique_ptr<FieldType> field);
            const FieldType* getField(const std::string& name) const;
            unsigned getFieldIndex(const std::string& fieldName) const;
            const std::unordered_map<std::string, unsigned>& getFieldNameToIndexMap() const { return fieldNameToIndex;} 
            const std::unordered_map<std::string, std::unique_ptr<FieldType>>& getFields() const { return fields; }

            // ---------------------
            // Parent Class Handling
            // ---------------------
            ClassType* getBaseClass() const { return parentClass; }
            void setBaseClass(ClassType* parent) { parentClass = parent; }
            bool hasBaseClass() const { return parentClass != nullptr; }
            std::vector<std::unique_ptr<MethodType>> getAllVirtualParentMethods() const;
            std::vector<std::string> getAllVirtualParentMethodsNames() const;

            // ---------------------
            // Interface Handling
            // ---------------------
            bool hasImplements(const InterfaceType* iface) const;
            bool hasImplements(const std::string& ifaceName) const;
            void addInterface(std::unique_ptr<InterfaceType> iface);
            bool hasInterfaces() const { return !interfaces.empty(); }
            inline const std::vector<std::unique_ptr<InterfaceType>>& getAllInterfaces() const { return interfaces; }; 
            

            // ---------------------
            // Mixin Handling
            // ---------------------
            bool usesMixin(const MixinType* mixin) const;
            bool usesMixin(const std::string& mixinName) const;
            void addMixin(std::unique_ptr<MixinType> mixin);
            bool hasMixins() const { return !mixins.empty(); }
            int getMixinOffset(const MixinType& mixin) const;
            const std::vector<std::unique_ptr<MixinType>>& getAllMixins() const { return mixins; };  
            std::vector<MixinOwner> getAllMixinMethods();

            /// ---------------------
            // Vtable Handling
            // ---------------------
            void bindVTable(llvm::Value* objValue);
            void buildVTable(const VTableType& vType);
            llvm::GlobalVariable* getOrCreateOrVTableGlobal() const;
            bool isVirtualFunction(const std::string& name) const;
            unsigned getVirtualMethodIndex(const std::string& name) const;
            llvm::Value* getVTablePtrPtr(llvm::Value* objValue) const;
            llvm::Value* loadVTablePtr(llvm::Value* objValue) const;
            llvm::Value* loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const;

            /// ---------------------
            //  UTILITIES
            // ---------------------
            std::string resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const;
            bool implementsInterface(const InterfaceType* iface) const;
            bool isSubclassOf(const ClassType* base) const;

            /// ---------------------
            // Debug Handling
            // ---------------------
            std::string getDebugName() const override { return "class"; }
            llvm::DIType* getDIType(llvm::DIScope* scope) const override;
            uint64_t getDebugSizeInBits() const override;
            uint32_t getDebugAlignInBits() const override;
            llvm::DINode::DIFlags getDIFlags() const override;

            ~ClassType() override = default;
    };

}

#endif
