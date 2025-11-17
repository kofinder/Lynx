#ifndef LYNX_TYPE_OPERANDS_VALIDATION_UTILS_HPP
#define LYNX_TYPE_OPERANDS_VALIDATION_UTILS_HPP

#include <llvm/IR/Value.h>

namespace LynxTypes {

    [[nodiscard]] inline bool validateOperands(llvm::Value* lhs, llvm::Value* lhsPtr, llvm::Value* rhs) noexcept {

        if (!lhs || !lhsPtr || !rhs) {
            llvm::errs() << "[Validation Failed] Null operand detected\n";
            return false;
        }
    
        if (lhs->getType() != rhs->getType()) {
            llvm::errs() << "[Validation Failed] LHS and RHS types do not match\n";
            return false;
        }
    
        if (!lhsPtr->getType()->isPointerTy()) {
            llvm::errs() << "[Validation Failed] lhsPtr is not a pointer\n";
            return false;
        }
    
        // if (lhs->getType() != llvm::Type::get<T>(lhs->getContext())) {
        //     llvm::errs() << "[Validation Failed] Operand type mismatch with template\n";
        //     return false;
        // }
    
        return true;
    }

    
        
}

#endif
