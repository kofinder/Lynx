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

            void initialize(std::unique_ptr<llvm::Module> module) override;

            int execute() override;

            ~ORCJITEngine() override = default;
    };
}

#endif
