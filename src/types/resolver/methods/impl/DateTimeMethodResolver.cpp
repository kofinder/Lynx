#include "methods/DateTimeMethodResolver.hpp"

namespace LynxTypes {

    llvm::Value* DateTimeMethodResolver::resolveMethod(
        const AstContext& /*ctx*/,
        llvm::Value* /*instace*/,
        llvm::Value* /*instancePtr*/,
        const std::string& /*methodName*/, 
        const std::vector<llvm::Value*>& /*args*/
    ) noexcept {
        
        return nullptr;
    }

}