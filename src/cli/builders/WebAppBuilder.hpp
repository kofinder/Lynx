#ifndef LYNX_CLI_WEB_APP_BUILDER_HPP
#define LYNX_CLI_WEB_APP_BUILDER_HPP


/**
 * @file WebAppBuilder.hpp
 * @brief Builder for web application projects.
 * 
 * Sets up frontend and backend folders, sample configuration files,
 * and initial code for web projects.
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

    class WebAppBuilder : public Builder {

        private:

            void createDirectories() {
                fs::create_directory(projectName);
                fs::create_directory(projectName + "/src");
                fs::create_directory(projectName + "/src/components");
                fs::create_directory(projectName + "/src/assets");
                fs::create_directory(projectName + "/libs");
                fs::create_directory(projectName + "/utils");
                fs::create_directory(projectName + "/constants");
                fs::create_directory(projectName + "/extensions");
                fs::create_directory(projectName + "/build");
            }
        
            void createFiles() {
                std::ofstream mainFile(projectName + "/main.lynx");
                mainFile << "// Entry point for web app\n";
                mainFile << "IO.println(\"Hello, WebApp!\");\n";
                mainFile.close();
        
                std::ofstream compFile(projectName + "/src/components/app_component.lynx");
                compFile << "// Example component\n";
                compFile << "IO.println(\"App Component Loaded\");\n";
                compFile.close();
        
                std::ofstream utilFile(projectName + "/utils/dom_utils.lynx");
                utilFile << "// DOM helper utilities\n";
                utilFile.close();
            }
        
            void setupConfigs() {
                std::ofstream configFile(projectName + "/app_config.yaml");

                if (!configFile.is_open()) {
                    throw std::runtime_error("Failed to create configuration file for project: " + projectName);
                }
            
                std::string kindStr = {"web"};
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
                std::cout << "Creating web app project: " << projectName << std::endl;
                createDirectories();
                createFiles();
                setupConfigs();
                std::cout << "Web app project '" << projectName << "' created successfully!\n";  
            }
    };

}


#endif
