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
        
            // Use explicit constructor for RAII
            explicit FunctionType(AstContext* context) : UserDefinedType(context) {}

            // Public copy constructor, needed for clone()
            FunctionType(const FunctionType& other) : UserDefinedType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~FunctionType() override = default;
            FunctionType& operator=(const FunctionType&) = delete;
            FunctionType(FunctionType&&) = delete;
            FunctionType& operator=(FunctionType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override { 
                return std::make_unique<FunctionType>(*this); 
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
         

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::BOOLEAN; }

            std::string getDebugName() const override { return "Function"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };

}

#endif
