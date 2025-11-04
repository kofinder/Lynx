#ifndef LYNX_SYSTEM_IO_MODULE_HPP
#define LYNX_SYSTEM_IO_MODULE_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"

namespace LynxSystem {

    class IOModule : public ISystemModule {

        public:

            IOModule();

            llvm::Value* invoke(std::shared_ptr<AstContext> context, const std::string& methodName, std::vector<llvm::Value*> calleeArgs) override;
        
        private:    

            using CommandFactory = std::function<std::unique_ptr<ISystemCommand>()>;

            std::unordered_map<std::string, CommandFactory> commands;
    };
        
    
}

#endif
