#ifndef LYNX_REFRENCE_TYPE_HPP
#define LYNX_REFRENCE_TYPE_HPP

#include <types/interfaces/WrapperType.hpp>

namespace LynxTypes {

    class ReferenceType : public WrapperType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit ReferenceType(AstContext* context) : WrapperType(context) {};

            inline DataType getTypeTag() const override { return DataType::REFERENCE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ReferenceType>(*this); }

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~ReferenceType() override = default;
    };
}

#endif
