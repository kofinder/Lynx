/**
 * @file BuiltInType.hpp
 * @brief Declares the BuiltInType class, the abstract base for all built-in primitive types.
 * 
 * The BuiltInType class extends the BaseType interface to represent all primitive data types 
 * directly supported by the Lynx language (e.g., integers, floating-point numbers, characters, strings).
 * It provides default behavior for built-in types, including assignment support, type compatibility checks, 
 * and LLVM size/alignment computation.
 * 
 * **Key Responsibilities:**
 * - Serves as the common superclass for all primitive (non-user-defined) types.
 * - Provides base implementations for assignment and conversion compatibility.
 * - Integrates with LLVM’s DataLayout for size and alignment information.
 * - Handles unsupported value creation signatures gracefully via runtime error reporting.
 * 
 * **Typical Derived Types:**
 * - IntType, FloatType, DoubleType, CharType, StringType, BoolType, etc.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BUILT_INS_TYPE_HPP
#define LYNX_BUILT_INS_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>
#include "utils/TypeResolverConstant.hpp"

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
    };
    
}

#endif 
