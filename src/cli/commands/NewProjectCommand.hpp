/**
 * @file NewModuleCommand.hpp
 * @brief Command to create a new module inside an existing project.
 * 
 * This command validates that the module does not already exist and
 * generates the directory structure and boilerplate for the module.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLI_NEW_PROJECT_COMMAND_HPP
#define LYNX_CLI_NEW_PROJECT_COMMAND_HPP

#include <string>
#include "factories/ProjectFactory.hpp"
#include "interfaces/ProjectKind.hpp"
#include "interfaces/Command.hpp"
#include <config/ProgramTerminalColor.hpp>

namespace LynxCLI {

    namespace fs = std::filesystem;

    class NewProjectCommand : public Command {

        private:

            std::string projectName;

            ProjectKind projectKind;

        public:

            NewProjectCommand(
                const std::string& name, 
                ProjectKind kind
            ) noexcept : projectName(name), projectKind(kind) {}
        
            void execute() override {
                if (fs::exists(projectName)) {
                    std::cerr << LYNX_RED  << "[Error] Project '" << projectName << "' already exists. Aborting creation.\n" << LYNX_RESET;
                    return;
                }
                auto builder = ProjectFactory::createBuilder(projectKind);
                builder->setProjectName(projectName);
                builder->build();  
            }

            std::string name() const noexcept override { return "create"; }

            std::string description() const noexcept override { return "Create a new Lynx project"; }
    };

}

#endif
