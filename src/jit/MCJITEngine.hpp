#ifndef LYNX_MCJIT_ENGINE_HPP
#define LYNX_MCJIT_ENGINE_HPP

#include "IJIT.hpp"
#include <iostream>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>

namespace LynxJIT {

    
    class MCJITEngine : public IJIT {

        private:
                
            std::vector<llvm::GenericValue> execArgs;

            std::unique_ptr<llvm::ExecutionEngine> engine;

            llvm::Function* execStartFunction = nullptr;

            void setArg(llvm::GenericValue argument);

        public:

            void initialize(std::unique_ptr<llvm::Module> module) override;

            int execute() override;

            ~MCJITEngine() override = default;

    };
}

#endif
