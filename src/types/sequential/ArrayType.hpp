#ifndef LYNX_ARRAY_TYPE_HPP
#define LYNX_ARRAY_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {
    

    class ArrayType : public SequentialType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;
            
        public:

            explicit ArrayType(AstContext* context) : SequentialType(context) {}
            
            inline bool isIndexable() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::ARRAY; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            llvm::Value* getElementPointer(llvm::Value* arrayAlloca, int index) const;
            
            llvm::Value* getElementPointer(llvm::Value* arrayAlloca, int outerIndex, int innerIndex) const;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ArrayType>(*this); }

            const BaseType* getElementType() const override { return elementType; }

            const size_t getNumElements() const { return numElements; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    
            ~ArrayType() override = default;
    };
}

#endif 
