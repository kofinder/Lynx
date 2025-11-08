#include "MCJITEngine.hpp"
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/DynamicLibrary.h>
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>

namespace LynxJIT {

    void MCJITEngine::initialize(std::unique_ptr<llvm::Module> module, const std::unordered_map<std::string, void*>& symbols) {
    
        std::string errStr;
        llvm::EngineBuilder builder(std::move(module));
        builder.setErrorStr(&errStr);
        builder.setEngineKind(llvm::EngineKind::JIT);
        builder.setMCJITMemoryManager(std::make_unique<llvm::SectionMemoryManager>());
        engine.reset(builder.create());
    
        if (!engine) {
            std::cerr << "Failed to create MCJIT ExecutionEngine: " << errStr << std::endl;
            throw std::runtime_error("Failed to create MCJIT ExecutionEngine");
        }
    
        // Register external runtime symbols so MCJIT can resolve them
        // You must register the runtime symbols manually for MCJIT
        for (const auto& [name, ptr] : symbols) {
            engine->addGlobalMapping(llvm::StringRef(name), reinterpret_cast<uint64_t>(ptr));
        }

        // Find entry function (e.g., "main")
        execStartFunction = engine->FindFunctionNamed("main");
        if (!execStartFunction) {
            module->print(llvm::errs(), nullptr);  // Dump IR to debug
            throw std::runtime_error("Entry function 'main' not found");
        }
    }    

    void MCJITEngine::setArg(llvm::GenericValue arg) {
        this->execArgs.push_back(arg);
    }

    int MCJITEngine::execute() {
        if (!engine || !execStartFunction) {
            std::cerr << "MCJITEngine not properly initialized" << std::endl;
            return -1;
        }
    
        // Prepare empty argument list (can be expanded to support args)
        execArgs.clear();
        llvm::GenericValue result = engine->runFunction(execStartFunction, execArgs);
        return static_cast<int>(result.IntVal.getZExtValue());
    }
}