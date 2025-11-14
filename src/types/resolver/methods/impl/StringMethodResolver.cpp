#include "methods/StringMethodResolver.hpp"

namespace LynxTypes {

    llvm::Value* StringMethodResolver::resolveMethod(
        AstContext& ctx,
        llvm::Value* instance,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}