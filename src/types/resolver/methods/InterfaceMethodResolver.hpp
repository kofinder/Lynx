#ifndef LYNX_INTERFACE_METHOD_RESOLVER_HPP
#define LYNX_INTERFACE_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    struct InterfaceMethodResolver : public TypeMethodResolver {
        
        llvm::Value* resolveMethod(
            const AstContext& ctx,
            llvm::Value* instance,
            llvm::Value* instancePtr,
            const std::string& method, 
            const std::vector<llvm::Value*>& args
        )  noexcept override;

    };
}

#endif
