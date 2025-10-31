#ifndef LYNX_CLI_NEW_MODULE_COMMAND_HPP
#define LYNX_CLI_NEW_MODULE_COMMAND_HPP

/**
 * @file NewProjectCommand.hpp
 * @brief Command to create a new project scaffold.
 * 
 * This command uses ProjectFactory to generate the project structure
 * based on the requested ProjectKind. It validates that the project
 * does not already exist before creation.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/


#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "interfaces/Command.hpp"

namespace LynxCLI {

    namespace fs = std::filesystem;

    class NewModuleCommand : public Command {

        private:

            std::string projectPath; 
            
            std::string moduleName; 

        public:

            NewModuleCommand(
                const std::string& project, 
                const std::string& module
            ) noexcept : projectPath(project), moduleName(module) {}

            void execute() override {
                if (!fs::exists(projectPath)) {
                    std::cerr << "Error: Project path does not exist!\n";
                    return;
                }

                fs::path moduleDir = fs::path(projectPath) / "src" / moduleName;
                if (!fs::exists(moduleDir)) {
                    fs::create_directories(moduleDir);
                    std::cout << "Created module directory: " << moduleDir << std::endl;
                } else {
                    std::cout << "Module directory already exists: " << moduleDir << std::endl;
                }
        
                fs::path moduleFile = moduleDir / (moduleName + ".lynx");
                if (!fs::exists(moduleFile)) {
                    std::ofstream outFile(moduleFile);
                    outFile << "// " << moduleName << " module\n";
                    outFile << "println(\"Module " << moduleName << " loaded\");\n";
                    outFile.close();
                    std::cout << "Created module file: " << moduleFile << std::endl;
                } else {
                    std::cout << "Module file already exists: " << moduleFile << std::endl;
                }       
        
            }

            std::string name() const noexcept override { return "generate"; }

            std::string description() const noexcept override { return "Generate a new Lynx module"; }
    };

}

#endif
