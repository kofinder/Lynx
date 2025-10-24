
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
            static inline std::unordered_map<const llvm::StructType*, ClassType*> llvmTypeToClass;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

            /**
             * @brief Determines if this class implements a given interface type.
             * 
             * Checks whether this class or any of its base classes declares the
             * specified interface in its list of implemented interfaces.
             * 
             * @param iface Pointer to the interface type to check against.
             * @return true if this class implements @p iface, false otherwise.
            */
            bool implementsInterface(const InterfaceType* iface) const;

            /**
             * @brief Determines if this class is a subclass of the given base class.
             *
             * Walk the parent chain to see whether @p base appears as this class's
             * ancestor. A class is considered a subclass of itself (returns true).
             *
             * @param base Pointer to the candidate base class.
             * @return true if this class inherits (directly or indirectly) from @p base.
            */
            bool isSubclassOf(const ClassType* base) const;

            template <typename MapT>
            std::vector<Candidate> findViableCandidatesImpl(const MapT& items, const std::vector<llvm::Type*>& argTypes) const;

            const std::vector<Candidate> findViableCandidates(MethodKind kind, const std::vector<llvm::Type*>& argTypes) const;

            int64_t scoreParameters(const std::vector<std::unique_ptr<BaseType>>& params, const std::vector<llvm::Type*>& argTypes) const;

        public:
        
            explicit ClassType(
                AstContext* context, 
                const std::string& name
            ) : UserDefinedType(context), className(name) {}

            const std::string& qualifiedName() const;
            const std::string& originalNameLower() const;
            const std::string& originalName() const { return className; }

            inline DataType getTypeTag() const override { return DataType::CLAZZ; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            std::unique_ptr<BaseType> clone() const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            void registerLLVMType(llvm::StructType* llvmStruct);
            static ClassType* fromLLVMType(const llvm::Type* type);

            // ---------------------
            // Constructor Handling
            // ---------------------
            void addConstructor(const std::string& mangledName, std::unique_ptr<ConstructorType> ctor);
            const ConstructorType* getConstructor(const std::string& mangledName) const;
            inline const std::unordered_map<std::string, std::unique_ptr<ConstructorType>>& getConstructors() const { return ctors; }
            
            // ---------------------
            // Method Handling
            // ---------------------
            bool hasMethod(const std::string& mangleName) const;
            const MethodType* getMethod(const std::string& mangleName) const;
            void addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method);
            inline const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getMethods() const { return methods; }

            // ---------------------
            // Fields Handling
            // ---------------------
            bool hasField(const std::string& name) const;
            void addField(const std::string& name, std::unique_ptr<FieldType> field);
            const FieldType* getField(const std::string& name) const;
            unsigned getFieldIndex(const std::string& fieldName) const;
            inline const std::unordered_map<std::string, unsigned>& getFieldNameToIndexMap() const { return fieldNameToIndex;} 
            inline const std::unordered_map<std::string, std::unique_ptr<FieldType>>& getFields() const { return fields; }

            // ---------------------
            // Parent Class Handling
            // ---------------------
            ClassType* getBaseClass() const { return parentClass; }
            void setBaseClass(ClassType* parent) { parentClass = parent; }
            bool hasBaseClass() const { return parentClass != nullptr; }
            std::vector<std::unique_ptr<MethodType>> getAllVirtualParentMethods() const;
            const std::vector<std::string> getAllVirtualParentMethodsNames() const;

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
            const std::vector<MixinOwner> getAllMixinMethods();

            /// @brief Initializes the vtable pointer for a given object.
            /// 
            /// This method should be called **once per object** (typically in
            /// the constructor). It stores a pointer to the class's global
            /// vtable into the first field of the object. Virtual method calls
            /// then use this vtable pointer for dispatch.
            ///
            /// @param objValue The LLVM value representing the object instance.
            void bindVTable(llvm::Value* objValue);
            void buildVTable(VTableType vType);
            llvm::GlobalVariable* getOrCreateOrVTableGlobal() const;

            bool isVirtualFunction(const std::string& name) const;
            unsigned getVirtualMethodIndex(const std::string& name) const;
            llvm::Value* getVTablePtrPtr(llvm::Value* objValue) const;
            llvm::Value* loadVTablePtr(llvm::Value* objValue) const;
            llvm::Value* loadVirtualMethodPtr(llvm::Value* vtablePtr, const std::string& fnName) const;

            // HELPERS
            std::string resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const;

            ~ClassType() override = default;
    };

}

#endif
