#include "methods/EnumMethodResolver.hpp"

namespace LynxTypes {

    llvm::Value* EnumMethodResolver::resolveMethod(
        const AstContext& /*ctx*/,
        llvm::Value* /*instace*/,
        llvm::Value* /*instancePtr*/,
        const std::string& /*methodName*/, 
        const std::vector<llvm::Value*>& /*args*/
    ) noexcept {
        
        return nullptr;
    }

}