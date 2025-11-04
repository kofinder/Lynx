#ifndef LYNX_SYSTEM_IO_PRINT_COMMAND_HPP
#define LYNX_SYSTEM_IO_PRINT_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"

namespace LynxSystem {

    class IOPrintCommand : public ISystemCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> ctx, std::vector<llvm::Value*> calleeArgs) override {
                return nullptr;
            }
    };
        
}

#endif
