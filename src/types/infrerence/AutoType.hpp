#ifndef LYNX_AUTO_TYPE_HPP
#define LYNX_AUTO_TYPE_HPP

#include <types/interfaces/BaseType.hpp>

namespace LynxTypes {

    class AutoType : public BaseType {

        private:

            BaseType* inferredType = nullptr;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
            explicit AutoType(AstContext* context) : BaseType(context) {};

            void accept(TypeVisitor& visitor) override;

            inline bool isBuiltInType() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::AUTO; }

            inline void setInferredType(BaseType* type) { inferredType = type; }
            
            inline BaseType* getInferredType() const { return inferredType; }
            
            inline bool isInferred() const { return inferredType != nullptr; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<AutoType>(*this); }
                        
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            bool canAccept(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~AutoType() override = default;
    };

}

#endif
