#ifndef LYNX_DATE_TIME_TYPE_HPP
#define LYNX_DATE_TIME_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class DateTimeType : public UserDefinedType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit DateTimeType(AstContext* context) : UserDefinedType(context) {}

            void accept(TypeVisitor& visitor) override;

            inline DataType getTypeTag() const override { return DataType::DATETIME; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* getField(std::string fieldName, llvm::Value* instance);

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DateTimeType>(*this); }

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~DateTimeType() override = default;
    };
}
#endif 
