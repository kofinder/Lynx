#ifndef LYNX_BUILT_INS_TYPE_HPP
#define LYNX_BUILT_INS_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>


using namespace LynxContext;

namespace LynxTypes {

    class BuiltInType : public BaseType {

        public: 

            explicit BuiltInType(AstContext* astContext) : BaseType(astContext) {}

            inline bool isBuiltInType() const  noexcept override { return true; }

            inline bool supportsAssignment() const noexcept override { return true; }

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override {
                astContext->reportError(makeRuntimeError(" createValue ( K, V) doesn't support this createValue signature."));
                return nullptr;  
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                auto o = dynamic_cast<const BuiltInType*>(other);
                if (!o) return false;
                switch (other->getTypeTag()) {
                    case DataType::BYTE:    return true;
                    case DataType::SHORT:   return true;
                    case DataType::INT:     return true;
                    case DataType::LONG:    return true;
                    case DataType::FLOAT:   return true;
                    case DataType::DOUBLE:  return true;
                    case DataType::CHAR:    return true;
                    case DataType::STRING:  return true;
                    default: return false;
                }

                return false;
            }

            virtual std::unique_ptr<BaseType> clone() const override = 0;

            ~BuiltInType() override = default;

        private:

            uint64_t getLLVMSizeInBits() const {
                llvm::Type* llvmType = getLLVMType();
                if (!llvmType) {
                    astContext->reportError(makeRuntimeError("LLVM Type is null in getLLVMSizeInBits()"));
                    return 0;
                }            
                auto& dl = astContext->getDataLayout();
                return dl.getTypeSizeInBits(llvmType);
            }
            
            uint32_t getLLVMAlignInBytes() const {
                llvm::Type* llvmType = getLLVMType();
                if (!llvmType) {
                    astContext->reportError(makeRuntimeError("LLVM Type is null in getLLVMAlignInBytes()"));
                    return 0;
                }            
                auto& dl = astContext->getDataLayout();
                return dl.getPrefTypeAlignment(llvmType);    
            }
        
    };
    
}

#endif 
