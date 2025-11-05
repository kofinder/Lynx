/**
 * @file IOModule.cpp
 * @brief Implements the IOModule class for managing I/O command execution.
 * 
 * The IOModule class provides an interface for executing input and output operations
 * within the Lynx system. It registers available I/O commands—such as `in` for reading 
 * input and `out` for printing output—and delegates execution to the corresponding 
 * command objects.
 * 
 * The module integrates with the AST execution context and LLVM IR builder, allowing 
 * I/O operations to be seamlessly compiled into LLVM IR instructions. Unrecognized 
 * method calls are logged as errors.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

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
