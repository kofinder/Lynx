#ifndef LYNX_SHORT_METHOD_RESOLVER_HPP
#define LYNX_SHORT_METHOD_RESOLVER_HPP

#include "TypeMethodResolver.hpp"

namespace LynxResolver {

    class ShortMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                const std::string& name, 
                llvm::Value* instance,
                const std::vector<llvm::Value*>& args,
                std::shared_ptr<AstContext> astContext
            ) override;

    };
}

#endif
