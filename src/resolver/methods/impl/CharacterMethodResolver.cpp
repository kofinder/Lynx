#include "methods/CharacterMethodResolver.hpp"

namespace LynxResolver {

    llvm::Value* CharacterMethodResolver::resolveMethod(const std::string& name,  llvm::Value* instance, const std::vector<llvm::Value*>& args, std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}