/**
 * @file GenericProjectBuilder.hpp
 * @brief Builder for a generic project scaffold.
 * 
 * Produces a simple default project layout with minimal configuration,
 * suitable for small utilities, libraries, or placeholders.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLI_GENERIC_PROJECT_BUILDER_HPP
#define LYNX_CLI_GENERIC_PROJECT_BUILDER_HPP

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "interfaces/Builder.hpp"


namespace LynxCLI {
    
    namespace fs = std::filesystem;

    class GenericProjectBuilder : public Builder {

        private:

            void createDirectories() {
                fs::create_directory(projectName);
                fs::create_directory(projectName + "/src");
                fs::create_directory(projectName + "/libs");
                fs::create_directory(projectName + "/utils");
                fs::create_directory(projectName + "/constants");
                fs::create_directory(projectName + "/extenstions");
            }
        
            void createFiles() {
                std::ofstream mainFile(projectName + "/main.lynx");
                mainFile << "// Entry point for your Lynx project\n";
                mainFile << "IO.println(\"Hello, Lynx!\");\n";
                mainFile.close();
        
                std::ofstream utilFile(projectName + "/utils/core_utility.lynx");
                utilFile << "// Utility functions go here\n";
                utilFile.close();
            }
        
            void setupConfigs() {
                std::ofstream configFile(projectName + "/app_config.yaml");

                if (!configFile.is_open()) {
                    throw std::runtime_error("Failed to create configuration file for project: " + projectName);
                }
            
                std::string kindStr = {"default"};
            
                configFile << "name: " << projectName << "\n";
                configFile << "type: " << kindStr << "\n";
                configFile << "version: 0.1.0\n";
                configFile << "entry: main.lynx\n\n";
            
                configFile << "folders:\n";
                configFile << "  - libs\n";
                configFile << "  - src\n";
                configFile << "  - utils\n";
                configFile << "  - constants\n";
                configFile << "  - extensions\n\n";
            
                configFile << "settings:\n";
                configFile << "  debug: true\n";
                configFile << "  optimize: false\n";
                configFile << "  build_dir: build\n\n";
            
                configFile << "exclude:\n";
                configFile << "  - test/\n";
                configFile << "  - experimental/\n\n";
            
                configFile << "flags:\n";
                configFile << "  optimize: true\n";
            
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
