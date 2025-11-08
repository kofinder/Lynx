/**
 * @file ORCJITEngine.hpp
 * @brief Implementation of IJIT using LLVM's ORC JIT backend.
 *
 * This header defines the ORCJITEngine class which wraps LLVM's ORC/LLJIT
 * execution engine to provide a unified JIT execution interface.
 *
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ORCJIT_ENGINE_HPP
#define LYNX_ORCJIT_ENGINE_HPP

#include "IJIT.hpp"
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/IR/Module.h>

namespace LynxJIT {

    class ORCJITEngine : public IJIT {

        private:
                
            std::unique_ptr<llvm::orc::LLJIT> orcJit;

        public:

            void initialize(std::unique_ptr<llvm::Module> module, const std::unordered_map<std::string, void*>& symbols = {}) override;

            int execute() override;

            ~ORCJITEngine() override = default;
    };
}

#endif
