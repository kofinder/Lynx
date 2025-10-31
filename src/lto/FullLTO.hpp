#ifndef LYNX_FULL_LTO_HPP
#define LYNX_FULL_LTO_HPP

#include <memory>
#include <string>

#include <logger/Logger.hpp>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/InitLLVM.h"
#include "passes/VTableExtractorPass.hpp"
#include "passes/AccessModifierPass.hpp"
#include "passes/UndenfinedBehaviorPass.hpp"
#include "strategy/DefaultLTOOptimizationStrategy.hpp"

using namespace LynxLogger;

namespace LynxLTO {

    class FullLTO : public DefaultLTOOptimizationStrategy {

        void optimize(llvm::Module &M) override {

            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed! Aborting optimization.\n";
                return;
            }
        
            llvm::PassBuilder PB;

            llvm::LoopAnalysisManager LAM;
            llvm::FunctionAnalysisManager FAM;
            llvm::CGSCCAnalysisManager CGAM;
            llvm::ModuleAnalysisManager MAM;
        
            // Register analyses
            PB.registerModuleAnalyses(MAM);
            PB.registerFunctionAnalyses(FAM);
            PB.registerLoopAnalyses(LAM);
            PB.registerCGSCCAnalyses(CGAM);
        
            // Cross-register proxies
            PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
        
            // Build and run pipeline
            auto defaultPipeline = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
            //PassPipeline.run(M, MAM);  
            
            // Build module pass manager and add passes
            llvm::ModulePassManager MPM;

            // 🧩 Add default pipeline
            MPM.addPass(std::move(defaultPipeline));

            // 🧩 Inject your custom pass **after optimization**
            MPM.addPass(VTableExtractorPass());
            MPM.addPass(AccessModifierPass());
            MPM.addPass(UndenfinedBehaviorPass());

            // 🛠 Run the combined pipeline
            MPM.run(M, MAM);

        }
        
    };
}

#endif 
