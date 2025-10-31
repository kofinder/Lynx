#ifndef LYNX_DATE_TYPE_HPP
#define LYNX_DATE_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class DateType : public UserDefinedType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit DateType(AstContext* context) : UserDefinedType(context) {}

            inline DataType getTypeTag() const override { return DataType::DATE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* getField(std::string fieldName, llvm::Value* instance);

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DateType>(*this); }

            ~DateType() override = default;
    };
}
#endif 
