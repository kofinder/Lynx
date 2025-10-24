#ifndef LYNX_THIN_LTO_HPP
#define LYNX_THIN_LTO_HPP

#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "strategy/DefaultLTOOptimizationStrategy.hpp"

using namespace LynxLogger;

namespace LynxLTO {

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
