#include "methods/InterfaceMethodResolver.hpp"

namespace LynxResolver {

    llvm::Value* InterfaceMethodResolver::resolveMethod(const std::string& name,  llvm::Value* instance, const std::vector<llvm::Value*>& args, std::shared_ptr<AstContext> astContext) noexcept {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}