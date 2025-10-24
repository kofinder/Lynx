#ifndef LYNX_VECTOR_TYPE_HPP
#define LYNX_VECTOR_TYPE_HPP

#include <unordered_set>
#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class VectorType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

            std::string getSafeStructName() const;

            std::string getSafeStructName(std::unordered_set<const BaseType*>& visited) const;

        public:

            explicit VectorType(AstContext* context) : SequentialType(context) {}

            inline bool isIndexable() const override { return true; }

            inline DataType getTypeTag() const override { return DataType::VECTOR; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            const BaseType* getElementType() const override { return elementType; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<VectorType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~VectorType() override = default;
    };
}

#endif
