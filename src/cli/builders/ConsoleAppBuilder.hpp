#ifndef LYNX_CLI_CONSOLE_APP_BUILDER_HPP
#define LYNX_CLI_CONSOLE_APP_BUILDER_HPP

/**
 * @file ConsoleAppBuilder.hpp
 * @brief Builder for console (CLI) projects.
 * 
 * Sets up a folder structure, main entry file, and example code
 * for console-based applications.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "interfaces/Builder.hpp"


namespace LynxCLI {
    
    namespace fs = std::filesystem;

    class ConsoleAppBuilder : public Builder {

        private:

            void createDirectories() {
                fs::create_directory(projectName);
                fs::create_directory(projectName + "/src");
                fs::create_directory(projectName + "/libs");
                fs::create_directory(projectName + "/utils");
                fs::create_directory(projectName + "/build");
            }

            void createFiles() {
                std::ofstream mainFile(projectName + "/main.lynx");
                mainFile << "// Entry point for console app\n";
                mainFile << "IO.println(\"Hello, Console App!\");\n";
                mainFile.close();

                std::ofstream utilFile(projectName + "/utils/io_utils.lynx");
                utilFile << "// Input/output helper functions\n";
                utilFile.close();
            }

            void setupConfigs() {
                std::ofstream configFile(projectName + "/app_config.yaml");
                configFile << "project_name: " << projectName << "\n";
                configFile << "type: console\n";
                configFile << "version: 0.1.0\n";
                configFile.close();
            }

        public:

            void build() override {
                std::cout << "Creating console app project: " << projectName << std::endl;
                createDirectories();
                createFiles();
                setupConfigs();
                std::cout << "Console app project '" << projectName << "' created successfully!\n";
            }

    };

}


#endif
