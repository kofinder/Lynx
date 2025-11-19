/**
 * @file MCJITEngine.hpp
 * @brief Implementation of IJIT using LLVM's MCJIT backend.
 *
 * This header defines the MCJITEngine class which wraps LLVM's MCJIT
 * execution engine to provide a unified JIT execution interface.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


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

            void initialize(std::unique_ptr<llvm::Module> module, const std::unordered_map<std::string, void*>& symbols = {}) override;

            int execute() override;

            ~MCJITEngine() override = default;

    };
}

#endif
