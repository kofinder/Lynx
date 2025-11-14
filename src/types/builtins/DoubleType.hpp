#ifndef LYNX_DOUBLE_TYPE_HPP
#define LYNX_DOUBLE_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class DoubleType: public BuiltInType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit DoubleType(AstContext* context) : BuiltInType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string, int>& getMethodRegistry() const override;

            llvm::Value* emitMethodCall(llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DoubleType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::DOUBLE; }
            
            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~DoubleType() override = default;
    };
}

#endif 
