/**
 * @file ConsoleAppBuilder.hpp
 * @brief Builder for console (CLI) projects.
 * 
 * Sets up a folder structure, main entry file, and example code
 * for console-based applications.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLI_CONSOLE_APP_BUILDER_HPP
#define LYNX_CLI_CONSOLE_APP_BUILDER_HPP

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
                createDirectories();
                createFiles();
                setupConfigs();
            }

    };

}


#endif
