
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

            static inline std::unordered_map<const llvm::StructType*, InterfaceType*> llvmTypeToClass;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit InterfaceType(
                AstContext* context, 
                std::string interfaceName
            ) : UserDefinedType(context), interfaceName(std::move(interfaceName))  {}

            const std::string& qualifiedName() const;
            const std::string& originalNameLower() const;
            const std::string& originalName() const { return interfaceName; }

            inline DataType getTypeTag() const override { return DataType::INTERFACE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool isAssignable(BaseType* lhsType, BaseType* rhsType);

            void addParentInterface(const InterfaceType* iface);

            std::unique_ptr<BaseType> clone() const override;
            
            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            void registerLLVMType(llvm::StructType* llvmStruct);
            static InterfaceType* fromLLVMType(const llvm::Type* type);

            bool hasMethod(const std::string& mangleName) const;
            void addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method);
            const MethodType* getMethod(const std::string& mangleName) const;
            std::vector<std::string> getMethodOrder() const;
            inline const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getMethods() const { return methods; }

            bool hasField(const std::string& name) const;
            void addField(const std::string& name, std::unique_ptr<FieldType> field);
            const FieldType* getField(const std::string& name) const;
            inline const std::unordered_map<std::string, unsigned>& getFieldNameToIndexMap() const { return fieldNameToIndex;} 
            inline const std::unordered_map<std::string, std::unique_ptr<FieldType>>& getFields() const { return fields; }
            inline const std::vector<const InterfaceType*>& getParents() const { return parentInterfaces; }

            void buildVTable(VTableType vType);
            llvm::GlobalVariable* getVTableGlobal() const;
            unsigned methodIndex(const std::string& name) const;
            llvm::GlobalVariable* getOrCreateOrVTableGlobal() const;
            llvm::Value* getVTablePtrPtr(llvm::Value* objValue) const;
            llvm::Value* loadVTablePtr(llvm::Value* objValue) const;
            llvm::Value* loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const;
            unsigned getVirtualMethodIndex(const std::string& methodName) const;

            std::string resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const;


            ~InterfaceType() override = default;
    };

}

#endif
