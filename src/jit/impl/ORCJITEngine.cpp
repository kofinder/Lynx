#include "jit/ORCJITEngine.hpp"

namespace LynxJIT {

    void ORCJITEngine::initialize(std::unique_ptr<llvm::Module> module,
        const std::unordered_map<std::string, void*>& symbols) {
        if (!module) throw std::runtime_error("LLVM module is null.");

        // Create LLJIT instance
        auto expectedJIT = llvm::orc::LLJITBuilder().create();
        if (!expectedJIT) {
            llvm::errs() << "LLJIT creation failed: " << llvm::toString(expectedJIT.takeError()) << "\n";
            throw std::runtime_error("Failed to create LLJIT instance.");
        }

        orcJit = std::move(*expectedJIT);

        // Mangle symbol names for target
        llvm::orc::MangleAndInterner mangle(orcJit->getExecutionSession(), orcJit->getDataLayout());

        // Register all external symbols from the host
        llvm::orc::SymbolMap interopSymbols;
        for (const auto& [name, ptr] : symbols) {
            // Convert host pointer to JIT target address
            llvm::JITTargetAddress addr = llvm::pointerToJITTargetAddress(ptr);

            // Wrap into ExecutorAddr
            llvm::orc::ExecutorAddr execAddr(addr);

            // Create ExecutorSymbolDef properly
            llvm::orc::ExecutorSymbolDef symDef(execAddr,
                llvm::JITSymbolFlags::Exported);

            // Insert into symbol map
            interopSymbols[mangle(name)] = symDef;
        }

        if (auto err = orcJit->getMainJITDylib().define(
            llvm::orc::absoluteSymbols(std::move(interopSymbols)))) {
            llvm::errs() << "Failed to register interop symbols: " << llvm::toString(std::move(err)) << "\n";
            throw std::runtime_error("Interop symbol registration failed.");
        }

        // Wrap the module in ThreadSafeModule
        auto tsc = std::make_unique<llvm::LLVMContext>();
        llvm::orc::ThreadSafeContext threadSafeCtx(std::move(tsc));
        llvm::orc::ThreadSafeModule tsm(std::move(module), threadSafeCtx);
        if (auto err = orcJit->addIRModule(std::move(tsm))) {
            llvm::errs() << "Error adding module: " << llvm::toString(std::move(err)) << "\n";
            throw std::runtime_error("Failed to add IR module to ORCJIT.");
        }
    }

    int ORCJITEngine::execute() {
        auto expectedAddr = orcJit->lookup("main");
        if (!expectedAddr) {
            llvm::errs() << "Symbol 'main' not found: " << llvm::toString(expectedAddr.takeError()) << "\n";
            throw std::runtime_error("Symbol 'main' not found.");
        }
    
        llvm::orc::ExecutorAddr addr = *expectedAddr;
        auto mainPtr = addr.toPtr<int (*)()>();
        return mainPtr();
    }
}
