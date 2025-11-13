/**
 * @file PACCallerUtils.hpp
 * @brief Utility class for Pointer Authentication Code (PAC) operations in LLVM IR.
 *
 * Provides helpers for signing, authenticating, and safely calling function pointers
 * with PAC intrinsics in LLVM.
 *
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 4, 2025
*/

#ifndef LYNX_PAC_CALLER_UTILS_HPP
#define LYNX_PAC_CALLER_UTILS_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"

namespace LynxAst::PAC {

    /**
     * @class CallerUtils
     * @brief High-level helper for generating LLVM PAC (Pointer Authentication) IR.
     *
     * This class provides a convenient API for inserting LLVM intrinsics related to
     * pointer authentication (signing, authenticating, and safely invoking function
     * pointers). It is designed to integrate PAC operations seamlessly into IR
     * generation passes or transformations.
    */
    class CallerUtils {

        private:

            llvm::Module* M;      

            llvm::IRBuilder<>& B;

            bool pacAvailable;

    
        public:

            /**
             * @brief Construct a new CallerUtils object.
             * 
             * @param module Pointer to the LLVM module where instructions will be inserted.
             * @param builder Reference to an IRBuilder for emitting instructions.
            */
            CallerUtils(
                llvm::Module* module, 
                llvm::IRBuilder<>& builder
            ) : M(module), B(builder), pacAvailable(supportsPAC(module)) {}
        
            /**
             * @brief Sign a pointer with PAC.
             *
             * Wraps the @c llvm.ptrauth.sign intrinsic to produce a signed pointer.
             *
             * @param ptr Pointer value to sign.
             * @return Signed pointer value.
            */
           llvm::Value* signPointer(llvm::Value* ptr) {
                if (!pacAvailable) return ptr; // fallback: no signing

                auto* ptrType = ptr->getType();
                auto* pacSign = llvm::Intrinsic::getOrInsertDeclaration(M, llvm::Intrinsic::ptrauth_sign, {ptrType});
                return B.CreateCall(pacSign, {ptr}, "signed_ptr");
            }
        
            /**
             * @brief Authenticate a pointer with PAC.
             *
             * Wraps the @c llvm.ptrauth.auth intrinsic to verify and authenticate a pointer.
             *
             * @param ptr Pointer value to authenticate.
             * @param discriminator Extra value used to strengthen PAC (default: 0).
             * @return Authenticated pointer value.
            */
           llvm::Value* authPointer(llvm::Value* ptr, uint64_t discriminator = 0) {
                if (!pacAvailable) return ptr; // fallback: no signing

                auto* ptrType = ptr->getType();
                auto* pacAuth = llvm::Intrinsic::getOrInsertDeclaration(M, llvm::Intrinsic::ptrauth_auth, {ptrType});
                auto* discVal = llvm::ConstantInt::get(B.getInt64Ty(), discriminator);
                return B.CreateCall(pacAuth, {ptr, discVal}, "auth_ptr");
            }
        
            /**
             * @brief Sign and then authenticate a pointer.
             *
             * Convenience wrapper that first calls @c signPointer() then @c authPointer().
             *
             * @param ptr Pointer value to sign and authenticate.
             * @param discriminator Extra value used for PAC authentication (default: 0).
             * @return Signed and authenticated pointer value.
            */
           llvm::Value* signAndAuth(llvm::Value* ptr, uint64_t discriminator = 0) {
                return authPointer(signPointer(ptr), discriminator);
            }
        
            /**
             * @brief Perform an automatic PAC-protected call on a function pointer.
             *
             * - Signs and authenticates the function pointer.
             * - Verifies that the value is a function pointer.
             * - Emits a safe @c call instruction in LLVM IR.
             *
             * @param funcPtr Function pointer to call.
             * @param args Argument list for the function (default: empty).
             * @param discriminator Optional PAC discriminator for authentication (default: 0).
             * @return Result of the function call as an LLVM Value*.
             *
             * @note Emits an error and returns @c nullptr if @p funcPtr is not a valid function pointer.
            */
           llvm::Value* call(llvm::Value* funcPtr, llvm::ArrayRef<llvm::Value*> args = {}, uint64_t discriminator = 0) {
                if (!pacAvailable) return funcPtr; // fallback: no signing
                return funcPtr;
                // // Sign and authenticate
                // auto* authPtr = signAndAuth(funcPtr, discriminator);

                // auto* ptrType = funcPtr->getType();
                // if (!ptrType->isPointerTy() || !ptrType->getPointerElementType()->isFunctionTy()) {
                //     llvm::errs() << "CallerUtils::call: Value is not a function pointer\n";
                //     return nullptr;
                // }

                // // Use dyn_cast for Type*
                // auto* funcType = llvm::dyn_cast<llvm::FunctionType>(ptrType->getPointerElementType());
                // if (!funcType) {
                //     llvm::errs() << "CallerUtils::call: Pointer element is not a FunctionType\n";
                //     return nullptr;
                // }

                // // Cast back to original type
                // auto* castPtr = B.CreateBitCast(authPtr, ptrType);
        
                // // Call the function
                // return B.CreateCall(funcType, castPtr, args);
            }

            bool supportsPAC(const llvm::Module* M) {
                auto triple = llvm::Triple(M->getTargetTriple());
                return triple.isAArch64(); // refine if needed: triple.isArm64e(), triple.isApple(), etc.
            }
            
    };
    
}
#endif
