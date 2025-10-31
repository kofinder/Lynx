#ifndef LYNX_CLI_COMMAND_REGISTRY_HPP
#define LYNX_CLI_COMMAND_REGISTRY_HPP

/**
 * @file CommandRegistry.hpp
 * @brief Singleton registry for all CLI commands.
 * 
 * The CommandRegistry manages the lifecycle of all available commands,
 * allowing registration and dynamic instantiation. Commands are stored
 * as factories to enable lazy creation on demand.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <iostream>
#include "interfaces/Command.hpp"

namespace LynxCLI {

    class CommandRegistry {

        public:

            using CommandFactory = std::function<std::unique_ptr<Command>()>;

            /**
             * @brief Get the singleton instance of the registry.
             * @return Reference to the CommandRegistry instance.
            */
            static CommandRegistry& instance() {
                static CommandRegistry registry;
                return registry;
            }

            /**
             * @brief Register a new command factory.
             * 
             * @param name The command name.
             * @param factory The factory function that creates the command.
             * 
             * This function stores the factory in the internal map,
             * allowing the CLI to instantiate commands dynamically.
            */
            void registerFactory(const std::string& name, CommandFactory factory) {
                factories[name] = std::move(factory);
            }
        
            /**
             * @brief Create a command by name.
             * 
             * @param name The name of the command.
             * @return std::unique_ptr<Command> Instance of the command if registered; nullptr otherwise.
            */
            std::unique_ptr<Command> create(const std::string& name) const {
                auto it = factories.find(name);
                if (it != factories.end()) {
                    return it->second();
                }
                return nullptr;
            }
        
            /**
             * @brief Print a list of all registered commands to stdout.
            */
            void listCommands() const {
                std::cout << "Available Commands:\n";
                for (const auto& [name, _] : factories) {
                    std::cout << "  - " << name << "\n";
                }
            }

        private:

            CommandRegistry() = default;
            
            ~CommandRegistry() = default;
            
            std::unordered_map<std::string, CommandFactory> factories;

    };

}

#endif
