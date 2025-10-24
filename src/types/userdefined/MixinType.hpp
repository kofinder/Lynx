
#ifndef LYNX_MIXIN_TYPE_HPP
#define LYNX_MIXIN_TYPE_HPP

#include "helper/FieldType.hpp"
#include "helper/MethodType.hpp"
#include "helper/VTableType.hpp"
#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class MixinType : public UserDefinedType {

        private:

            std::string mixinName;

            mutable llvm::StructType* cachedType = nullptr;

            mutable llvm::PointerType* cachedPointerType = nullptr;

            mutable std::string cachedFullName;

            mutable std::string cachedLowerName;

            std::vector<const MixinType*> parentMixins;

            std::unordered_map<std::string, std::unique_ptr<MethodType>> methods;

            std::unordered_map<std::string, std::unique_ptr<FieldType>> fields;

            mutable std::unordered_map<std::string, unsigned> fieldNameToIndex;
            
            std::unordered_map<std::string, unsigned> methodNameToIndex;

            static inline std::unordered_map<const llvm::StructType*, MixinType*> llvmTypeToClass;

            // Flattened members after semantic analysis
            std::vector<std::unique_ptr<FieldType>> flattenedFields;
            std::unordered_map<std::string, std::unique_ptr<MethodType>> flattenedMethods;
            std::vector<std::string> flattenedMethodOrder; // preserve insertion order for LLVM layout


        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit MixinType(
                AstContext* context, 
                std::string name
            ) : UserDefinedType(context), mixinName(std::move(name))  {}

            const std::string& qualifiedName() const;
            const std::string& originalNameLower() const;
            const std::string& originalName() const { return mixinName; }

            inline DataType getTypeTag() const override { return DataType::MIXIN; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool isAssignable(BaseType* lhsType, BaseType* rhsType);

            std::unique_ptr<BaseType> clone() const override;
            
            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            void registerLLVMType(llvm::StructType* llvmStruct);
            static MixinType* fromLLVMType(const llvm::Type* type);

            bool hasParentMixins() const { return !parentMixins.empty(); }
            void addParentMixin(const MixinType* mixin);

            bool hasMethod(const std::string& mangleName) const;
            void addMethod(const std::string& mangleName, std::unique_ptr<MethodType> method);
            const MethodType* getMethod(const std::string& mangleName) const;
            unsigned methodIndex(const std::string& name) const;
            inline const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getMethods() const { return methods; }

            bool hasField(const std::string& name) const;
            void addField(const std::string& name, std::unique_ptr<FieldType> field);
            const FieldType* getField(const std::string& name) const;
            inline const std::unordered_map<std::string, unsigned>& getFieldNameToIndexMap() const { return fieldNameToIndex;} 
            inline const std::unordered_map<std::string, std::unique_ptr<FieldType>>& getFields() const { return fields; }
            inline const std::vector<const MixinType*>& getParents() const { return parentMixins; }

            // Returns a pointer to the super mixin instance that implements a specific method
            llvm::Value* resolveSuperInstanceForMethod(const std::string& methodName, llvm::Value* thisPtr, std::vector<llvm::Type*> argumentTypes) const;
            llvm::Value* resolveSuperInstanceForField(const std::string& fieldName) const;
            std::string resolveMethodCall(MethodKind kind, const std::string& mangledName, const std::vector<llvm::Type*>& argTypes) const;


            // semantic analysis
            const std::vector<std::unique_ptr<FieldType>>& getFlattenedFields() const { return flattenedFields; }
            void setFlattenedFields(const std::unordered_map<std::string, FieldType*>& fieldsMap);
            const std::unordered_map<std::string, std::unique_ptr<MethodType>>& getFlattenedMethods() const { return flattenedMethods; }
            void setFlattenedMethods(const std::unordered_map<std::string, MethodType*>& methodsMap, const std::vector<std::string>& methodOrder = {});
            const std::vector<std::string>& getFlattenedMethodOrder() const { return flattenedMethodOrder; }
            const MethodType* getFlattenedMethod(const std::string& sig) const;

            ~MixinType() override = default;
    };

}

#endif
