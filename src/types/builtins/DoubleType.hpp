#ifndef LYNX_DOUBLE_TYPE_HPP
#define LYNX_DOUBLE_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class DoubleType : public BuiltInType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit DoubleType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            DoubleType(const DoubleType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~DoubleType() override = default;
            DoubleType& operator=(const DoubleType&) = delete;
            DoubleType(DoubleType&&) = delete;
            DoubleType& operator=(DoubleType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<DoubleType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return doubleMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::DOUBLE; }
            
            std::string getDebugName() const override { return "double"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif 
