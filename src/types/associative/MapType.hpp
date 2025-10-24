#ifndef LYNX_MAP_TYPE_HPP
#define LYNX_MAP_TYPE_HPP

#include <types/interfaces/AssociativeType.hpp>

namespace LynxTypes {

    class MapType : public AssociativeType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit MapType(AstContext* context) : AssociativeType(context) {}

            inline bool isAssociative() const override { return true; }

            inline DataType getTypeTag() const override { return DataType::MAP; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<MapType>(*this);}

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            const BaseType* getElementType() const override { return elementType; }

            void setValueType(BaseType* value) override { elementValue = value; }

            const BaseType* getValueType() const override { return elementValue; }
                        
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~MapType() override = default;
    };
}

#endif
