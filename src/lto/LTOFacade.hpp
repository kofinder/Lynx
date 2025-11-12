/**
 * @file LTOFacade.hpp
 * @brief Provides a facade for performing Link-Time Optimization (LTO) on LLVM modules.
 *
 * The `LTOFacade` class simplifies the process of applying LTO optimizations to LLVM
 * modules. It integrates an optimization strategy, handles target machine setup, and
 * provides methods to emit optimized LLVM bitcode or assembly files.
 *
 * **Key Responsibilities:**
 * - Apply LTO optimizations using a pluggable strategy (`DefaultLTOOptimizationStrategy`).
 * - Emit LLVM bitcode to files.
 * - Emit assembly files for the host target.
 * - Configure target machine and data layout automatically.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_LTO_FACADE_HPP
#define LYNX_LTO_FACADE_HPP

#include <iostream>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <llvm/Support/Host.h> 
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Linker/Linker.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h"
#include <llvm/IR/LegacyPassManager.h>
#include "strategies/DefaultLTOOptimizationStrategy.hpp"

namespace LynxLTO {

    class LTOFacade {
        
        private:

            llvm::LLVMContext &context;

            std::unique_ptr<DefaultLTOOptimizationStrategy> strategy;

            static void writeBitcodeToFile(llvm::Module &M, llvm::raw_fd_ostream &OS) {
                llvm::WriteBitcodeToFile(M, OS);
            }

        public:

            LTOFacade(
                llvm::LLVMContext &ctx, 
                std::unique_ptr<DefaultLTOOptimizationStrategy> stgy
            ) : context(ctx), strategy(std::move(stgy)) {
                std::cout << "LTO Facade constructor initialization" << std::endl;
            }

            void optimize(llvm::Module &M) {
                strategy->optimize(M);
            }

            void emitAssembly(llvm::Module &M, const std::string &filename) {
                auto targetTriple = llvm::sys::getDefaultTargetTriple();
                std::string error;
                const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
            
                if (!target) {
                    llvm::errs() << "Failed to lookup target: " << error << "\n";
                    return;
                }
            
                llvm::TargetOptions opt;
                auto RM = llvm::Optional<llvm::Reloc::Model>();
                std::unique_ptr<llvm::TargetMachine> targetMachine(
                    target->createTargetMachine(targetTriple, "generic", "", opt, RM)
                );
            
                M.setDataLayout(targetMachine->createDataLayout());
                M.setTargetTriple(targetTriple);
            
                std::error_code EC;
                llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);
            
                if (EC) {
                    llvm::errs() << "Could not open file: " << EC.message() << "\n";
                    return;
                }
            
                llvm::legacy::PassManager pass;
                if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_AssemblyFile)) {
                    llvm::errs() << "TargetMachine can't emit a file of this type\n";
                    return;
                }
            
                pass.run(M);
                dest.flush();            
            }

            void emitBitcode(llvm::Module &M, const std::string &filename) {
                std::error_code ec;
                llvm::raw_fd_ostream OS(filename, ec, llvm::sys::fs::OF_None);
                if (ec) {
                    llvm::errs() << "Error opening file " << filename << ": " << ec.message() << "\n";
                    return;
                }

                writeBitcodeToFile(M, OS);
                OS.flush();
            }
    };
}

#endif 
