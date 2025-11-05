/**
 * @file Command.hpp
 * @brief Abstract base class for all CLI commands.
 * 
 * This interface defines the core behavior for CLI commands in the Lynx project.
 * All commands must implement the execute method and provide their name and description.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CLI_COMMAND_HPP
#define LYNX_CLI_COMMAND_HPP

#include <string>

namespace LynxCLI {

    class Command {

        public:

            /**
             * @brief Execute the command.
             * 
             * Derived classes must implement this method to perform the specific
             * command action.
            */
            virtual void execute() = 0;

            /**
             * @brief Returns the name of the command.
             * @return std::string Command name.
            */
            virtual std::string name() const noexcept = 0;

            /**
             * @brief Returns a short description of the command.
             * @return std::string Command description.
            */
            virtual std::string description() const noexcept = 0;
        
            /**
             * @brief Virtual destructor.
            */
            virtual ~Command() noexcept = default;

    };

}

#endif