/**
 * @file AdvancedLTOOptimizationStrategy.hpp
 * @brief Implements an advanced LTO optimization strategy using LLVM passes.
 *
 * The `AdvancedLTOOptimizationStrategy` class extends `DefaultLTOOptimizationStrategy`
 * to provide a highly optimized LLVM pass pipeline. It applies interprocedural,
 * function-level, loop, scalar, and vectorization optimizations to improve
 * performance and reduce code size for compiled modules.
 *
 * **Key Responsibilities:**
 * - Apply module-level interprocedural optimizations (e.g., GlobalDCE, MergeFunctions, Inliner).
 * - Apply function-level optimizations (e.g., EarlyCSE, Reassociate, JumpThreading, ADCE, LICM).
 * - Apply loop transformations and vectorization (LoopUnroll, LoopVectorize, SLPVectorizer).
 * - Coordinate analyses via LLVM's PassBuilder and pass managers.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

#ifndef LYNX_ADVANCED_LTO_OPTIMIZATION_STRATEGY_HPP
#define LYNX_ADVANCED_LTO_OPTIMIZATION_STRATEGY_HPP

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

// Scalar optimizations
#include "llvm/Transforms/Scalar/SROA.h"
#include "llvm/Transforms/Scalar/EarlyCSE.h"
// #include "llvm/Transforms/Scalar/InstCombine.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
// #include "llvm/Transforms/Scalar/DSE.h"
#include "llvm/Transforms/Scalar/ADCE.h"
#include "llvm/Transforms/Scalar/LICM.h"
#include "llvm/Transforms/Scalar/LoopUnrollPass.h"
#include "llvm/Transforms/Scalar/JumpThreading.h"
// #include "llvm/Transforms/Scalar/TailCallElim.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

#include "llvm/Transforms/IPO/Inliner.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/ConstantMerge.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/IPO/MergeFunctions.h"
// #include "llvm/Transforms/IPO/PostOrderFunctionAttrs.h"

#include "llvm/Transforms/Vectorize/LoopVectorize.h"
#include "llvm/Transforms/Vectorize/SLPVectorizer.h"
#include "DefaultLTOOptimizationStrategy.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;

namespace LynxLTO {

    class AdvancedLTOOptimizationStrategy : public DefaultLTOOptimizationStrategy {

        public:

            void optimize(llvm::Module &M) override {

                llvm::PassBuilder PB;

                llvm::LoopAnalysisManager LAM;
                llvm::FunctionAnalysisManager FAM;
                llvm::CGSCCAnalysisManager CGAM;
                llvm::ModuleAnalysisManager MAM;

                PB.registerModuleAnalyses(MAM);
                PB.registerCGSCCAnalyses(CGAM);
                PB.registerFunctionAnalyses(FAM);
                PB.registerLoopAnalyses(LAM);
                PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

                llvm::ModulePassManager MPM;

                // === Module-Level Interprocedural Optimizations ===
                MPM.addPass(llvm::GlobalDCEPass());
                MPM.addPass(llvm::ConstantMergePass());
                MPM.addPass(llvm::MergeFunctionsPass());
                // MPM.addPass(llvm::IPSCCPPass());
                MPM.addPass(llvm::InlinerPass());
                // MPM.addPass(llvm::PostOrderFunctionAttrsPass());

                // === Function-Level Pipeline ===
                llvm::FunctionPassManager FPM;

                // FPM.addPass(llvm::SROA());
                FPM.addPass(llvm::EarlyCSEPass());
                // FPM.addPass(llvm::InstCombinePass());
                FPM.addPass(llvm::ReassociatePass());
                FPM.addPass(llvm::JumpThreadingPass());
                // FPM.addPass(llvm::DSEPass());
                FPM.addPass(llvm::ADCEPass());
                // FPM.addPass(llvm::TailCallElimPass());
                // FPM.addPass(llvm::LICMPass());
                FPM.addPass(llvm::LoopSimplifyPass());
                FPM.addPass(llvm::LoopUnrollPass());
                FPM.addPass(llvm::LoopVectorizePass());
                FPM.addPass(llvm::SLPVectorizerPass());

                // Wrap FPM in CGSCC, then wrap that in MPM
                llvm::CGSCCPassManager CGPM;
                // CGPM.addPass(llvm::FunctionToLoopPassAdaptor(std::move(FPM)));
                MPM.addPass(llvm::createModuleToPostOrderCGSCCPassAdaptor(std::move(CGPM)));

                // Run on module
                MPM.run(M, MAM);
            }
    };

} 

#endif 
