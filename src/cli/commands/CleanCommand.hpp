/**
 * @file CleanCommand.hpp
 * @brief Command to clean up build artifacts and temporary files.
 * 
 * This command removes intermediate build files, generated modules,
 * and any cached outputs to reset the project to a clean state.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CLI_CLEAN_COMMAND_HPP
#define LYNX_CLI_CLEAN_COMMAND_HPP

#include <iostream>
#include "interfaces/Command.hpp"

namespace LynxCLI {

    class CleanCommand : public Command {

        public:

            CleanCommand() noexcept = default;

            void execute() override {
                std::cout << "[CleanCommand] Cleaning build artifacts..." << std::endl;
                int result = system("make clean");
                if (result != 0) {
                    std::cerr << "[CleanCommand] Clean failed with code: " << result << std::endl;
                } else {
                    std::cout << "[CleanCommand] Clean completed successfully." << std::endl;
                }
            }

            std::string name() const noexcept override { return "clean"; }
            
            std::string description() const noexcept override { return "Clean up build files"; }
    };

}

#endif