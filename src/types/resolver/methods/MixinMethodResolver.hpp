#ifndef LYNX_MIXIN_METHOD_RESOLVER_HPP
#define LYNX_MIXIN_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    class MixinMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                AstContext& ctx,
                llvm::Value* instance,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept override;

    };
}

#endif
