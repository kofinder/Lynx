#include "methods/DoubleMethodResolver.hpp"

namespace LynxResolver {

    llvm::Value* DoubleMethodResolver::resolveMethod(const std::string& name,  llvm::Value* instance, const std::vector<llvm::Value*>& args, std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Invoked...................");
        return nullptr;
    }

}