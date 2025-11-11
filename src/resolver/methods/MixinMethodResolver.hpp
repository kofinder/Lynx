#ifndef LYNX_MIXIN_METHOD_RESOLVER_HPP
#define LYNX_MIXIN_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class MixinMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                const std::string& name, 
                llvm::Value* instance,
                const std::vector<llvm::Value*>& args,
                std::shared_ptr<AstContext> astContext
            ) noexcept override;

    };
}

#endif
