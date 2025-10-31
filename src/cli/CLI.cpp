#include "CLI.hpp"
#include <iostream>

#include "commands/BuildCommand.hpp"
#include "commands/CleanCommand.hpp"
#include "commands/HelpCommand.hpp"
#include "commands/NewProjectCommand.hpp"
#include "commands/NewModuleCommand.hpp"
#include "commands/CommandRegistry.hpp"
#include <config/ProgramOptionConfig.hpp>

namespace LynxCLI {

    using LynxProgramConfig::ProgramOptionConfig;

    void CLI::registerCommands() const {
        auto& registry = CommandRegistry::instance();

        registry.registerFactory("create", []() {
            auto& config = ProgramOptionConfig::instance();
            auto projectName = config.options()["project"].as<std::string>();
            auto projectType = config.options().count("type") ?
                                      config.options()["type"].as<std::string>() : "generic";
            return std::make_unique<NewProjectCommand>(
                projectName, parseStringToProjectKind(projectType)
            );
        });

        registry.registerFactory("generate", []() {
            auto& config = ProgramOptionConfig::instance();
            auto subcommand = config.options()["subcommand"].as<std::string>();
            auto name = config.options()["name"].as<std::string>();
            if (subcommand == "module") {
                return std::make_unique<NewModuleCommand>(".", name);
            }
            throw std::invalid_argument("Unknown generate subcommand: " + subcommand);
        });

        registry.registerFactory("build", []() {
            return std::make_unique<BuildCommand>();
        });

        registry.registerFactory("clean", []() {
            return std::make_unique<CleanCommand>();
        });

        registry.registerFactory("help", []() {
            return std::make_unique<HelpCommand>();
        });
    }

    void CLI::handleUnknownCommand(const std::string& cmd) const noexcept {
        std::cerr << "[Error] Unknown command: '" << cmd << "'\n";
        std::cout << "Available commands:\n";
        CommandRegistry::instance().listCommands();
    }
    
    void CLI::run() {

        registerCommands();

        auto& config = ProgramOptionConfig::instance();
        const std::string cmd = config.getCommand();

        if (cmd.empty()) {
            std::cerr << "[Error] No command specified.\n";
            return;
        }

        auto command = CommandRegistry::instance().create(cmd);
        if (!command) {
            handleUnknownCommand(cmd);
            return;
        }

        command->execute();
    }
}