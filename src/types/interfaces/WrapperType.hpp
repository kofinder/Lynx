#ifndef LYNX_WRAPPER_TYPE_HPP
#define LYNX_WRAPPER_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    class WrapperType : public BaseType {

        public: 

            explicit WrapperType(AstContext* astContext) : BaseType(astContext) {}

            inline bool isWrapperType() const override { return true; }

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
                auto o = dynamic_cast<const WrapperType*>(other);
                if (!o) return false;

                return false;
            }

            virtual std::unique_ptr<BaseType> clone() const override = 0;

            ~WrapperType() override = default;
    };
}

#endif
