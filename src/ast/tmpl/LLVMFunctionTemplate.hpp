/**
 * @file LLVMFunctionTemplate.hpp
 * @brief Declares LLVM function utilities for applying attributes and calling 
 *        conventions in the Lynx compiler's code generation pipeline.
 * 
 * The LLVMFunctionTemplate utilities provide helper functions to configure LLVM 
 * function metadata, including attributes and calling conventions. These tools 
 * are used by the Lynx compiler backend to ensure correct function behavior, 
 * optimization hints, and ABI conformance during LLVM IR generation.
 * 
 * **Key Responsibilities:**
 * - Apply LLVM function attributes based on `FunctionAttributeType` flags.
 * - Convert internal calling convention types to LLVM calling conventions.
 * - Automatically infer and apply appropriate calling conventions via 
 *   `CallingConventionInferer`.
 * 
 * **Used By:**
 * - Code generation passes for function definition and declaration.
 * - Backend compilation components managing LLVM IR function creation.
 * 
 * @see FunctionAttributeType, CallingConventionType, CallingConventionInferer
 * 
 * @note The utilities in this file provide a centralized mechanism for function 
 *       attribute and calling convention management. They ensure consistency 
 *       between user-defined attributes and LLVM IR-level semantics.
 * 
 * @namespace LynxAst
 * Provides LLVM IR-related utilities for applying function attributes and 
 * calling conventions during code generation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_LLVM_FUNCTION_TEMPLATE_HPP
#define LYNX_LLVM_FUNCTION_TEMPLATE_HPP


#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Casting.h>
#include <constants/FunctionAttributeType.hpp>
#include <constants/CallingConventionType.hpp>
#include <callconv/CallingConventionInferer.hpp>


namespace LynxAst {
    
    using namespace LynxConstants;
    using namespace LynxCallConv;
    
    void applyFunctionAttributes(llvm::Function* func, const std::vector<FunctionAttributeType>& attrs) {
        for (auto attr : attrs) {
            switch (attr) {
                case FunctionAttributeType::ALWAYS_INLINE:
                    func->addFnAttr(llvm::Attribute::AlwaysInline);
                    break;
                case FunctionAttributeType::NO_INLINE:
                    func->addFnAttr(llvm::Attribute::NoInline);
                    break;
                case FunctionAttributeType::NO_RETURN:
                    func->addFnAttr(llvm::Attribute::NoReturn);
                    break;
                case FunctionAttributeType::READ_ONLY:
                    func->addFnAttr(llvm::Attribute::ReadOnly);
                    break;
                case FunctionAttributeType::READ_NONE:
                    func->addFnAttr(llvm::Attribute::ReadNone);
                    break;
                case FunctionAttributeType::NO_CAPTURE:
                    func->addFnAttr(llvm::Attribute::NoCapture);
                    break;
                case FunctionAttributeType::NO_UNWIND:
                    func->addFnAttr(llvm::Attribute::NoUnwind);
                    break;
                case FunctionAttributeType::SANITIZE_ADDRESS:
                    func->addFnAttr("sanitize_address");
                    break;
                case FunctionAttributeType::OPTIMIZE_FOR_SIZE:
                    func->addFnAttr(llvm::Attribute::OptimizeForSize);
                    break;
                case FunctionAttributeType::INLINE_HINT:
                    func->addFnAttr(llvm::Attribute::InlineHint);
                    break;
                case FunctionAttributeType::STACK_PROTECTOR:
                    func->addFnAttr(llvm::Attribute::StackProtect);
                    break;
                case FunctionAttributeType::NO_INLINE_HINT:
                    func->addFnAttr(llvm::Attribute::NoInline);
                    break;
                case FunctionAttributeType::NO_DEAD_STRIP:
                    func->addFnAttr("no_dead_strip");
                    break;
                case FunctionAttributeType::INTRINSIC:
                    func->addFnAttr("intrinsic");
                    break;
                case FunctionAttributeType::NO_RECURSE:
                    func->addFnAttr("no_recurse");
                    break;
                case FunctionAttributeType::SANITIZE_THREAD:
                    func->addFnAttr("sanitize_thread");
                    break;
                case FunctionAttributeType::SANITIZE_MEMORY:
                    func->addFnAttr("sanitize_memory");
                    break;
                case FunctionAttributeType::SANITIZE_HWADDRESS:
                    func->addFnAttr("sanitize_hwaddress");
                    break;
                case FunctionAttributeType::NO_INLINE_ASM:
                    func->addFnAttr("no_inline_asm");
                    break;
                case FunctionAttributeType::UW_TABLE:
                    func->addFnAttr("uwtable");
                    break;
                case FunctionAttributeType::ZERO_EXTEND:
                    func->addFnAttr(llvm::Attribute::ZExt);
                    break;
                case FunctionAttributeType::SIGN_EXTEND:
                    func->addFnAttr(llvm::Attribute::SExt);
                    break;
                case FunctionAttributeType::ARGMEMONLY:
                    func->addFnAttr("argmemonly");
                    break;
                case FunctionAttributeType::RETURNS_TWICE:
                    func->addFnAttr(llvm::Attribute::ReturnsTwice);
                    break;
                case FunctionAttributeType::STRICT_FP:
                    func->addFnAttr("strictfp");
                    break;
                case FunctionAttributeType::SANITIZE_COVERAGE:
                    func->addFnAttr("sanitize_coverage");
                    break;
                case FunctionAttributeType::ASSUME_SAME_ALIGNED:
                    func->addFnAttr("assume_sane_aligned");
                    break;
                case FunctionAttributeType::NO_SANITIZE_MEMORY:
                    func->addFnAttr("no_sanitize_memory");
                    break;
                case FunctionAttributeType::NO_SANITIZE_THREAD:
                    func->addFnAttr("no_sanitize_thread");
                    break;
                case FunctionAttributeType::NO_SANITIZE_ADDRESS:
                    func->addFnAttr("no_sanitize_address");
                    break;
                default:
                    func->addFnAttr(llvm::Attribute::None);
                    break;
            }
        }
    }

    llvm::CallingConv::ID toLLVMCallingConv(CallingConventionType cc) {
        switch(cc) {
            case CallingConventionType::C: return llvm::CallingConv::C;
            case CallingConventionType::Fast: return llvm::CallingConv::Fast;
            case CallingConventionType::Cold: return llvm::CallingConv::Cold;
            case CallingConventionType::GHC: return llvm::CallingConv::GHC;
            case CallingConventionType::HiPE: return llvm::CallingConv::HiPE;
            case CallingConventionType::AnyReg: return llvm::CallingConv::AnyReg;
            case CallingConventionType::PreserveMost: return llvm::CallingConv::PreserveMost;
            case CallingConventionType::PreserveAll: return llvm::CallingConv::PreserveAll;
            case CallingConventionType::CXXFastTLS: return llvm::CallingConv::CXX_FAST_TLS;
            case CallingConventionType::Tail: return llvm::CallingConv::Tail;
            case CallingConventionType::Swift: return llvm::CallingConv::Swift;
            case CallingConventionType::SwiftTail: return llvm::CallingConv::SwiftTail;
            case CallingConventionType::CFGuardCheck: return llvm::CallingConv::CFGuard_Check;
            case CallingConventionType::StdCall: return llvm::CallingConv::X86_StdCall;
            case CallingConventionType::FastCall: return llvm::CallingConv::X86_FastCall;
            case CallingConventionType::ThisCall: return llvm::CallingConv::X86_ThisCall;
            case CallingConventionType::Numbered:
            case CallingConventionType::PreserveNone:
            default: return llvm::CallingConv::C;
        }
    }

    void applyCallingConvention(llvm::Function* func, CallingConventionType convType) {
        CallingConventionInferer inferer;
        auto inferred = inferer.infer(func);
        func->setCallingConv(toLLVMCallingConv(inferred));
        LOG_ERROR("Applied calling convention {} to function {}", static_cast<int>(convType), func->getName().str());
    }
}

#endif 


    