#include "methods/CharacterMethodResolver.hpp"

namespace LynxTypes {

    llvm::Value* CharacterMethodResolver::resolveMethod(
        const AstContext& ctx,
        llvm::Value* instance,
        llvm::Value* instancePtr,
        const std::string& method, 
        const std::vector<llvm::Value*>& args
    ) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}