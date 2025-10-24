#ifndef LYNX_BYTE_TYPE_HPP
#define LYNX_BYTE_TYPE_HPP

#include  <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {
    
    class ByteType: public BuiltInType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
            explicit ByteType(AstContext* context) : BuiltInType(context) {}

            void accept(TypeVisitor& visitor) override;

            inline DataType getTypeTag() const override { return DataType::BYTE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;

            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ByteType>(*this); }

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~ByteType() override = default;
    };
}

#endif 
