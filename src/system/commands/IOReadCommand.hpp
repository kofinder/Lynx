#ifndef LYNX_SYSTEM_IO_READ_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"
#include "utils/SystemIO.hpp"
#include <context/AstContext.hpp>

namespace LynxSystem {

    class IOReadCommand : public ISystemCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> ctx, std::vector<llvm::Value*> calleeArgs) override {

                std::cout << "Execute" << std::endl;

                auto& builder = ctx->getBuilder();
                auto* module = ctx->getModule();
                llvm::Value* format = builder.CreateGlobalStringPtr("%d", "scanf_fmt");

                return SystemIO::createPrintCall(builder, module, format);
            }
    };
        
}

#endif
