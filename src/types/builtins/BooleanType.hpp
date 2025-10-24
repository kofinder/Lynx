#ifndef LYNX_BOOLEAN_TYPE_HPP
#define LYNX_BOOLEAN_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class BooleanType: public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit BooleanType(AstContext* context) : BuiltInType(context) {}

            inline DataType getTypeTag() const override { return DataType::BOOLEAN; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<BooleanType>(*this); }

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* convertBooleanToString(llvm::Value* value);

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~BooleanType() override = default;
    };
}

#endif 
