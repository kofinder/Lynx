#ifndef LYNX_CLI_HPP
#define LYNX_CLI_HPP

/**
 * @file CLI.hpp
 * @brief Command Line Interface (CLI) for the Lynx project.
 * 
 * The CLI class manages parsing of user input, registering available commands,
 * and executing them based on the provided arguments. It integrates with the
 * CommandRegistry singleton to dynamically instantiate and run commands like
 * create, generate, build, clean, and help.
 * 
 * Responsibilities:
 *  - Register all available CLI commands.
 *  - Validate and dispatch commands from user input.
 *  - Handle unknown or unsupported commands gracefully.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/
#include <string>

namespace LynxCLI {

    class CLI {

        private:

            /**
             * @brief Registers all commands with the CommandRegistry.
             * 
             * Each command is registered with a lambda that creates the appropriate
             * command instance when executed.
            */
            void registerCommands() const;

            /**
             * @brief Handles unknown or unsupported commands.
             * 
             * Prints an error message and lists available commands.
             * @param cmd The unrecognized command string.
            */
            void handleUnknownCommand(const std::string& cmd) const noexcept;

        public:

            CLI() = default;     

            void run(); 

            ~CLI() = default;
    };

}

#endif
