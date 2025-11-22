/**
 * @file WrapperType.hpp
 * @brief Declares the WrapperType base class for all type wrappers (e.g., Option, Ref, Smart Pointer types).
 *
 * The WrapperType class serves as an abstraction for all types that encapsulate or wrap another
 * underlying type. Examples include optional types (`Option<T>`), reference types (`Ref<T>`), or
 * smart pointer abstractions (`Shared<T>`, `Unique<T>`).
 *
 * **Key Responsibilities:**
 * - Provides a unified interface for types that wrap other types.
 * - Supports const and static qualifiers inherited from BaseType.
 * - Defines core behaviors for conversion, cloning, and type compatibility.
 * - Integrates with LLVM IR type creation and code generation pipelines through the BaseType interface.
 *
 * **Intended Derivatives:**
 * - OptionType
 * - RefType
 * - PointerType
 * - SmartPointerType
 *
 * **Design Notes:**
 * - This class differentiates wrapper semantics (ownership, mutability, optionality)
 *   from regular user-defined or primitive types.
 * - Must be subclassed with concrete implementations of `clone()` and `computeLLVMType()`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_WRAPPER_TYPE_HPP
#define LYNX_WRAPPER_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    using namespace LynxContext;

    class WrapperType : public BaseType {

        public: 

            explicit WrapperType(AstContext* astContext) : BaseType(astContext) {}
            ~WrapperType() override = default;

            bool isWrapperType() const noexcept override { return true; }

            llvm::Value* createValue(std::vector<llvm::Value*> /*unused*/) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*unused*/) const override {
                astContext->reportError(makeRuntimeError(" createValue ( K, V) doesn't support this createValue signature."));
                return nullptr;  
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                const auto* obj = dynamic_cast<const WrapperType*>(other);
                if (!obj) return false;
                return false;
            }
    };
}

#endif
