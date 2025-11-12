/**
 * @file ThinLTO.hpp
 * @brief Implements Thin Link-Time Optimization (ThinLTO) for LLVM modules.
 *
 * The `ThinLTO` class extends `DefaultLTOOptimizationStrategy` to provide
 * ThinLTO-specific optimization passes for LLVM modules. It verifies modules
 * before and after optimization and uses LLVM's legacy pass manager and
 * PassManagerBuilder to apply ThinLTO passes.
 *
 * **Key Responsibilities:**
 * - Verify LLVM modules before and after optimization.
 * - Apply ThinLTO passes using LLVM's PassManagerBuilder.
 * - Integrate seamlessly with the LTOFacade optimization workflow.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_THIN_LTO_HPP
#define LYNX_THIN_LTO_HPP

#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "strategies/DefaultLTOOptimizationStrategy.hpp"

namespace LynxLTO {

    using namespace LynxLogger;

    class ThinLTO : public DefaultLTOOptimizationStrategy {

        void optimize(llvm::Module &M) override {
            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed! Aborting optimization.\n";
                return;
            }

            // Create legacy pass manager to run ThinLTO passes
            llvm::legacy::PassManager PM;
    
            // You can use PassManagerBuilder for IPO passes, with ThinLTO enabled
            llvm::PassManagerBuilder PMBuilder;
            PMBuilder.OptLevel = 3;
            PMBuilder.SizeLevel = 0;
            PMBuilder.populateLTOPassManager(PM);
    
            PM.run(M);
    
            // Verify module again after optimization
            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed after optimization!\n";
                return;
            }
        }
        
    };
}

#endif 
