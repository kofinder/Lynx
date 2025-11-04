#include "IOModule.hpp"
#include "commands/IOPrintCommand.hpp"
#include "commands/IOReadCommand.hpp"

namespace LynxSystem {

    IOModule::IOModule() {
        commands["in"]  = []() { return std::make_unique<IOReadCommand>(); };
        commands["out"] = []() { return std::make_unique<IOPrintCommand>(); };
    }

    llvm::Value* IOModule::invoke(std::shared_ptr<AstContext> context,const std::string& methodName, std::vector<llvm::Value*> calleeArgs) {
        auto it = commands.find(methodName);
        if (it == commands.end()) {
            LOG_ERROR("Unknown IO method: {}", methodName);
            return nullptr;
        }

        auto command = it->second();
        return command->execute(context, calleeArgs);
    }

}