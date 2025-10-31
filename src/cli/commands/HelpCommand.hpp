#ifndef LYNX_CLI_HELP_COMMAND_HPP
#define LYNX_CLI_HELP_COMMAND_HPP

/**
 * @file HelpCommand.hpp
 * @brief Command to display help information for available CLI commands.
 * 
 * This command lists all registered commands and provides usage
 * information. It can also show detailed help for specific commands
 * if arguments are provided.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/

#include <iostream>
#include "CommandRegistry.hpp"
#include "interfaces/Command.hpp"

namespace LynxCLI {

    class HelpCommand : public Command {

        public:

            HelpCommand() noexcept = default;

            void execute() override {
                std::cout << "Available commands:\n";
                auto& registry = CommandRegistry::instance();
                registry.listCommands();
            }

            std::string name() const noexcept override { return "help"; }
            
            std::string description() const noexcept override { return "Show help message"; }
    };

}

#endif
