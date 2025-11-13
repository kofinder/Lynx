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

#include "llvm/IR/Verifier.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

#include "passes/AccessModifierPass.hpp"
#include "passes/AttributorPass.hpp"
#include "passes/DeadCodeEliminationPass.hpp"
#include "passes/DevirtualizeFunctionCallsPass.hpp"
#include "passes/DevirtualizePass.hpp"
#include "passes/FunctionDocDumperPass.hpp"
#include "passes/FunctionDocExtractorPass.hpp"
#include "passes/FunctionInliningPass.hpp"
#include "passes/FunctionMetadataPass.hpp"
#include "passes/FunctionSecurityPass.hpp"
#include "passes/FunctionSignatureAuditPass.hpp"
#include "passes/GlobalDCEPPass.hpp"
#include "passes/PointerCaptureAnalysisPass.hpp"
#include "passes/RemoveUnusedParamsPass.hpp"
#include "passes/RequireAnalysisPass.hpp"
#include "passes/TargetIRVerifierPass.hpp"
#include "passes/UndenfinedBehaviorPass.hpp"
#include "passes/VirtualCallOptimizationPass.hpp"
#include "passes/VTableExtractorPass.hpp"
#include "passes/WholeProgramDevirtPass.hpp"
#include "strategies/DefaultLTOOptimizationStrategy.hpp"

namespace LynxLTO {

    class ThinLTO : public DefaultLTOOptimizationStrategy {

        void optimize(llvm::Module& M) override {

            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed! Aborting optimization.\n";
                return;
            }
        
            // Setup analysis managers
            llvm::LoopAnalysisManager       LAM;
            llvm::FunctionAnalysisManager   FAM;
            llvm::CGSCCAnalysisManager      CGAM;
            llvm::ModuleAnalysisManager     MAM;
            llvm::PassBuilder               PB;
        
            PB.registerModuleAnalyses(MAM);
            PB.registerCGSCCAnalyses(CGAM);
            PB.registerFunctionAnalyses(FAM);
            PB.registerLoopAnalyses(LAM);
            PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

            auto MPM = PB.buildModuleOptimizationPipeline(
                llvm::OptimizationLevel::O3,
                llvm::ThinOrFullLTOPhase::ThinLTOPreLink
            );

            MPM.addPass(AccessModifierPass());
            MPM.addPass(AttributorPass());
            MPM.addPass(llvm::createModuleToFunctionPassAdaptor(DeadCodeEliminationPass()));
            MPM.addPass(DevirtualizeFunctionCallsPass());
            MPM.addPass(DevirtualizePass());
            MPM.addPass(FunctionDocDumperPass());
            MPM.addPass(FunctionDocExtractorPass());
            MPM.addPass(FunctionInliningPass());
            MPM.addPass(FunctionMetadataPass());
            MPM.addPass(FunctionSecurityPass());
            MPM.addPass(FunctionSignatureAuditPass());
            MPM.addPass(GlobalDCEPPass());
            MPM.addPass(PointerCaptureAnalysisPass());
            MPM.addPass(llvm::createModuleToFunctionPassAdaptor(RemoveUnusedParamsPass()));
            MPM.addPass(RequireAnalysisPass());
            MPM.addPass(TargetIRVerifierPass());
            MPM.addPass(UndenfinedBehaviorPass());
            MPM.addPass(VirtualCallOptimizationPass());
            MPM.addPass(VTableExtractorPass());
            MPM.addPass(WholeProgramDevirtPass());

            llvm::errs() << "[LynxLTO] Running ThinLTO optimization pipeline...\n";

            MPM.run(M, MAM);
        
            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed after optimization!\n";
                return;
            }

            llvm::errs() << "[LynxLTO] ThinLTO optimization complete.\n";
        }
        
    };
}

#endif 
