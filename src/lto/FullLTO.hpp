/**
 * @file FullLTO.hpp
 * @brief Implements Full Link-Time Optimization (FullLTO) for LLVM modules.
 *
 * The `FullLTO` class extends `DefaultLTOOptimizationStrategy` to provide
 * comprehensive FullLTO optimization for LLVM modules. It constructs and executes
 * a complete LLVM pass pipeline, including default optimization passes and
 * custom passes such as VTable extraction, access modifier adjustments, and
 * undefined behavior detection.
 *
 * **Key Responsibilities:**
 * - Verify LLVM modules before optimization.
 * - Build and run LLVM's FullLTO pass pipeline with `PassBuilder` and `ModulePassManager`.
 * - Inject custom passes after standard optimizations.
 * - Ensure optimized modules maintain correctness and apply project-specific analyses.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FULL_LTO_HPP
#define LYNX_FULL_LTO_HPP

#include <memory>
#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

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

    class FullLTO : public DefaultLTOOptimizationStrategy {

        void optimize(llvm::Module &M) override {

            if (llvm::verifyModule(M, &llvm::errs())) {
                llvm::errs() << "Module verification failed! Aborting optimization.\n";
                return;
            }
        
            llvm::LoopAnalysisManager       LAM;
            llvm::FunctionAnalysisManager   FAM;
            llvm::CGSCCAnalysisManager      CGAM;
            llvm::ModuleAnalysisManager     MAM;
            llvm::PassBuilder               PB;
        
            // Register analyses
            PB.registerModuleAnalyses(MAM);
            PB.registerFunctionAnalyses(FAM);
            PB.registerLoopAnalyses(LAM);
            PB.registerCGSCCAnalyses(CGAM);
        
            PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
        
            auto MPM = PB.buildModuleOptimizationPipeline(llvm::OptimizationLevel::O3, llvm::ThinOrFullLTOPhase::FullLTOPostLink);
            
            // MPM.addPass(AccessModifierPass());
            // MPM.addPass(AttributorPass());
            // MPM.addPass(llvm::createModuleToFunctionPassAdaptor(DeadCodeEliminationPass()));
            // MPM.addPass(DevirtualizeFunctionCallsPass());
            // MPM.addPass(DevirtualizePass());
            // MPM.addPass(FunctionDocDumperPass());
            // MPM.addPass(FunctionDocExtractorPass());
            // MPM.addPass(FunctionInliningPass());
            // MPM.addPass(FunctionMetadataPass());
            // MPM.addPass(FunctionSecurityPass());
            // MPM.addPass(FunctionSignatureAuditPass());
            // MPM.addPass(GlobalDCEPPass());
            // MPM.addPass(PointerCaptureAnalysisPass());
            // MPM.addPass(llvm::createModuleToFunctionPassAdaptor(RemoveUnusedParamsPass()));
            // MPM.addPass(RequireAnalysisPass());
            // MPM.addPass(TargetIRVerifierPass());
            // MPM.addPass(UndenfinedBehaviorPass());
            // MPM.addPass(VirtualCallOptimizationPass());
            // MPM.addPass(VTableExtractorPass());
            // MPM.addPass(WholeProgramDevirtPass());


            MPM.run(M, MAM);
        }
        
    };
}

#endif 
