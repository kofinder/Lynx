/**
 * @file BuildCommand.hpp
 * @brief Command to build the current project.
 * 
 * This command triggers compilation, linking, and any additional
 * build steps defined for the project. It integrates with the
 * Lynx build system.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLI_BUILD_COMMAND_HPP
#define LYNX_CLI_BUILD_COMMAND_HPP

#include <iostream>
#include "interfaces/Command.hpp"

namespace LynxCLI {

    class BuildCommand : public Command {
        
        public:
        
            BuildCommand() noexcept = default;

            void execute() override {
                std::cout << "[BuildCommand] Building the project..." << std::endl;
                int result = system("make build");
                if (result != 0) {
                    std::cerr << "[BuildCommand] Build failed with code: " << result << std::endl;
                } else {
                    std::cout << "[BuildCommand] Build succeeded." << std::endl;
                }
            }

            std::string name() const noexcept override { return "build"; }
            
            std::string description() const noexcept override { return "Compile the project"; }
    };

}

#endif