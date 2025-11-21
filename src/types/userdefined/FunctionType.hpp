#ifndef LYNX_FUNCTION_TYPE_HPP
#define LYNX_FUNCTION_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class FunctionType : public UserDefinedType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
            explicit FunctionType(AstContext* context) : UserDefinedType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
         
            std::unique_ptr<BaseType> clone() const override { return std::make_unique<FunctionType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::BOOLEAN; }

            std::string getDebugName() const override { return "Function"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~FunctionType() override = default;
    };

}

#endif
