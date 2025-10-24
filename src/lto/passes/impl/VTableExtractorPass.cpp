#include <passes/VTableExtractorPass.hpp>
#include "llvm/Support/JSON.h" 
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include <llvm/Support/FileSystem.h>
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    llvm::PreservedAnalyses VTableExtractorPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running LynxVTableExtractorPass on module: " << M.getName() << "\n";
    
        llvm::errs() << "Module named metadata nodes:\n";
        for (auto &NMD : M.named_metadata()) {
            llvm::errs() << "  " << NMD.getName() << " (" << NMD.getNumOperands() << " operands)\n";
        }
    
        llvm::NamedMDNode *vtables = M.getNamedMetadata("lynx.vtables");
        if (!vtables) {
            llvm::errs() << "[Lynx] No vtables found.\n";
            return llvm::PreservedAnalyses::all();
        }
    
        llvm::errs() << "[Lynx] Found " << vtables->getNumOperands() << " vtable metadata nodes.\n";
    
        llvm::json::Object outJson;
    
        for (const llvm::MDNode *node : vtables->operands()) {
            if (node->getNumOperands() != 2) {
                llvm::errs() << "[Lynx] Skipping metadata node with operand count " << node->getNumOperands() << "\n";
                continue;
            }
    
            auto *ifaceNameMD = llvm::dyn_cast<llvm::MDString>(node->getOperand(0));
            auto *methodsList = llvm::dyn_cast<llvm::MDNode>(node->getOperand(1));
            if (!ifaceNameMD || !methodsList) {
                llvm::errs() << "[Lynx] Invalid metadata node format, skipping\n";
                continue;
            }
    
            std::string ifaceName = ifaceNameMD->getString().str();
            llvm::json::Array methodsJson;
    
            for (const auto &methodMDVal : methodsList->operands()) {
                auto *methodMD = llvm::dyn_cast<llvm::MDNode>(methodMDVal);
                if (!methodMD || methodMD->getNumOperands() < 2) {
                    llvm::errs() << "[Lynx] Skipping invalid method metadata\n";
                    continue;
                }
    
                auto *name = llvm::dyn_cast<llvm::MDString>(methodMD->getOperand(0));
                auto *sig = llvm::dyn_cast<llvm::MDString>(methodMD->getOperand(1));
    
                if (name && sig) {
                    llvm::json::Object methodJson;
                    methodJson["name"] = name->getString().str();
                    methodJson["signature"] = sig->getString().str();
                    methodsJson.push_back(std::move(methodJson));
                } else {
                    llvm::errs() << "[Lynx] Method metadata missing name or signature\n";
                }
            }
            outJson.try_emplace(ifaceName, std::move(methodsJson));
        }
    
        if (outJson.empty()) {
            llvm::errs() << "[Lynx] No valid vtable metadata extracted\n";
            return llvm::PreservedAnalyses::all();
        }
    
        llvm::json::Value jsonVal = llvm::json::Value(std::move(outJson));
        const std::string filePath = "livechat/build/vtable_metadata.json";

        // Print JSON to console nicely formatted
       llvm::outs() << "[Lynx Reflection JSON]\n";
       // llvm::outs() << llvm::formatv("{0:2}", jsonVal) << "\n";
    
        // Save JSON to file in project directory
        std::error_code EC;
        llvm::raw_fd_ostream fileOS(filePath, EC, llvm::sys::fs::OF_Text);
        if (EC) {
            llvm::errs() << "[Lynx] Failed to open vtable_metadata.json for writing: " << EC.message() << "\n";
        } else {
            fileOS << llvm::formatv("{0:2}", jsonVal);
            fileOS.close();
            llvm::errs() << "[Lynx] vtable_metadata.json saved successfully.\n";
        }
    
        // Preserve all analyses, since we do not modify IR here
        return llvm::PreservedAnalyses::all();
    }
}
