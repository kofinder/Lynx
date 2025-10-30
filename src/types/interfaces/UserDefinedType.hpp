#ifndef LYNX_USER_DEFINED_TYPE_HPP
#define LYNX_USER_DEFINED_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    /**
     * @class UserDefinedType
     * @brief Represents a user-defined structure or object with named fields.
     * 
     * Allows defining custom types like records, structs, or class-like entities.
     * Supports introspection and member access.
    */
    class UserDefinedType : public BaseType {

        public:
        
            explicit UserDefinedType(AstContext* context) : BaseType(context) {}

            inline bool isUserDefinedType() const noexcept override { return true; }

            llvm::Value* createValue(LValueType value) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

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
                auto o = dynamic_cast<const UserDefinedType*>(other);
                if (!o) return false;
                switch (other->getTypeTag()) {
                    case DataType::CLAZZ:    return true;
                    case DataType::INTERFACE:   return true;
                    case DataType::DATE:   return true;
                    case DataType::DATETIME:   return true;
                    case DataType::ENUM:   return true;
                    case DataType::FILE:   return true;
                    case DataType::FUNCTION:   return true;
                    default: return false;
                }

                return false;
            }

            virtual std::unique_ptr<BaseType> clone() const override = 0;
    
            virtual ~UserDefinedType() override = default;
    };
}

#endif
