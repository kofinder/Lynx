#include "ORCJITEngine.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/ExecutionEngine/Orc/ExecutorProcessControl.h>
#include <llvm/ExecutionEngine/Orc/Shared/TargetProcessControlTypes.h>


namespace LynxJIT {

    void ORCJITEngine::initialize(std::unique_ptr<llvm::Module> module, const std::unordered_map<std::string, void*>& symbols) {
        if (!module) throw std::runtime_error("LLVM module is null.");
    
        auto expectedJIT = llvm::orc::LLJITBuilder().create();
        if (!expectedJIT) {
            llvm::errs() << "LLJIT creation failed: " << llvm::toString(expectedJIT.takeError()) << "\n";
            throw std::runtime_error("Failed to create LLJIT instance.");
        }
    
        orcJit = std::move(*expectedJIT);
    
        // Mangle symbol names to match target
        llvm::orc::MangleAndInterner mangle(orcJit->getExecutionSession(), orcJit->getDataLayout());
    
        // ✅ Register all external symbols from InteropManager
        llvm::orc::SymbolMap interopSymbols;
        for (const auto& [name, ptr] : symbols) {
            interopSymbols[mangle(name)] = llvm::JITEvaluatedSymbol(
                llvm::pointerToJITTargetAddress(ptr),
                llvm::JITSymbolFlags::Exported
            );
        }
    
        if (auto err = orcJit->getMainJITDylib().define(llvm::orc::absoluteSymbols(std::move(interopSymbols)))) {
            llvm::errs() << "Failed to register interop symbols: " << llvm::toString(std::move(err)) << "\n";
            throw std::runtime_error("Interop symbol registration failed.");
        }
    
        // Wrap the module
        llvm::orc::ThreadSafeContext tsc(std::make_unique<llvm::LLVMContext>());
        llvm::orc::ThreadSafeModule tsm(std::move(module), std::move(tsc));
    
        if (auto err = orcJit->addIRModule(std::move(tsm))) {
            llvm::errs() << "Error adding module: " << llvm::toString(std::move(err)) << "\n";
            throw std::runtime_error("Failed to add IR module to ORCJIT.");
        }
    }
    
    
    int ORCJITEngine::execute() {
        auto mainSym = orcJit->lookup("main");
        if (!mainSym) {
            llvm::errs() << "Symbol 'main' not found: " << llvm::toString(mainSym.takeError()) << "\n";
            throw std::runtime_error("Symbol 'main' not found.");
        }
        
        auto mainPtr = (int (*)())(intptr_t)(mainSym->getAddress());
        return mainPtr();
    }

}
