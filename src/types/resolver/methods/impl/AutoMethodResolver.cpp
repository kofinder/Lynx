#include "methods/AutoMethodResolver.hpp"

namespace LynxTypes {

    llvm::Value* AutoMethodResolver::resolveMethod(
        const AstContext& /*ctx*/,
        llvm::Value* /*instace*/,
        llvm::Value* /*instancePtr*/,
        const std::string& /*methodName*/, 
        const std::vector<llvm::Value*>& /*args*/
    ) noexcept {
        
        return nullptr;
    }

}