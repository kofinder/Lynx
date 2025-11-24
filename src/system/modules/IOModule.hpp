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
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_MODULE_HPP
#define LYNX_SYSTEM_IO_MODULE_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"
#include "tmpl/SystemModuleTemplate.hpp"

// IO WRITE
#include "commands/io/impl/IOPrintOutCommand.hpp"
#include "commands/io/impl/IOPrintfCommand.hpp"
#include "commands/io/impl/IOPrintlnCommand.hpp"

// IO READ
#include "commands/io/impl/IOReadLineCommand.hpp"
#include "commands/io/impl/IOReadByteCommand.hpp"
#include "commands/io/impl/IOReadShortCommand.hpp"
#include "commands/io/impl/IOReadIntCommand.hpp"
#include "commands/io/impl/IOReadLongCommand.hpp"
#include "commands/io/impl/IOReadFloatCommand.hpp"
#include "commands/io/impl/IOReadDoubleCommand.hpp"
#include "commands/io/impl/IOReadCharCommand.hpp"
#include "commands/io/impl/IOReadBoolCommand.hpp"
#include "commands/io/impl/IOReadMapCommand.hpp"
#include "commands/io/impl/IOReadArrayCommand.hpp"
#include "commands/io/impl/IOReadObjectCommand.hpp"


namespace LynxSystem {

    using namespace LynxSystem::meta;

    class IOModule final: public ISystemModule {

        public:

            IOModule() {
                
                // write
                registerCommand<IOPrintOutCommand>("out", commands); 
                registerCommand<IOPrintfCommand>("printf", commands);   
                registerCommand<IOPrintlnCommand>("println", commands);

                // read
                registerCommand<IOReadLineCommand>("in", commands);
                registerCommand<IOReadByteCommand>("getByte", commands);
                registerCommand<IOReadShortCommand>("getShort", commands);
                registerCommand<IOReadIntCommand>("getInt", commands);
                registerCommand<IOReadLongCommand>("getLong", commands);
                registerCommand<IOReadFloatCommand>("getFloat", commands);
                registerCommand<IOReadDoubleCommand>("getDouble", commands);
                registerCommand<IOReadCharCommand>("getChar", commands);
                registerCommand<IOReadBoolCommand>("getBool", commands);
                registerCommand<IOReadMapCommand>("getMap", commands);
                registerCommand<IOReadArrayCommand>("getArray", commands);
                registerCommand<IOReadObjectCommand>("getObject", commands);    
            }

            llvm::Value* invokeCommand(std::shared_ptr<AstContext> context, const std::string& methodName, std::vector<llvm::Value*> calleeArgs) override {
                auto it = commands.find(methodName);
                if (it == commands.end()) {
                    LOG_ERROR("Unknown IO method: {}", methodName);
                    return nullptr;
                }
                auto command = it->second();
                return command->execute(context, calleeArgs);        
            }
        
            ~IOModule() noexcept override = default;

        private:    

            using CommandFactory = std::function<std::unique_ptr<ISystemCommand>()>;

            std::unordered_map<std::string, CommandFactory> commands;
    };
    
}

#endif
