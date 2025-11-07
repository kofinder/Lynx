/**
 * @file SystemModuleTemplate.hpp
 * @brief Template utility for registering commands in system modules.
 * 
 * This header defines a type-safe helper for registering command factories
 * inside Lynx system modules. It uses modern C++20 features like `concepts`
 * and `constexpr` to ensure correctness at compile time.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

 #ifndef LYNX_SYSTEM_MODULE_TEMPLATE_HPP
 #define LYNX_SYSTEM_MODULE_TEMPLATE_HPP
 
 #include <string>
 #include <memory>
 #include <functional>
 #include <unordered_map>
 #include <concepts>
 #include "system/ISystemCommand.hpp"
 
 namespace LynxSystem::meta {
 
    /**
     * @brief Concept that validates a type as a valid system command.
     *
     * A valid command type must derive publicly from `ISystemCommand`.
     * This ensures that only compatible command classes can be registered
     * through the `registerCommand()` helper.
     */
     template <typename T>
     concept SystemCommandType = std::derived_from<T, ISystemCommand>;

    /**
     * @brief Concept that validates a compatible command factory map type.
     *
     * This ensures that the provided container supports string keys and
     * can store callable factories returning `std::unique_ptr<ISystemCommand>`.
     * Typical use: `std::unordered_map<std::string, CommandFactory>`.
     */
     template <typename M>
     concept CommandMapType = requires(M m, const std::string& key, std::unique_ptr<ISystemCommand> value) {
        { m[key] } -> std::same_as<typename M::mapped_type&>;
     };
 
 
    /**
     * @brief Registers a command factory within a system module.
     *
     * Associates the given command name with a factory lambda that
     * constructs instances of `T`. If a command with the same name
     * already exists, a warning is logged and the previous entry
     * is overwritten.
     *
     * @tparam T  The command type; must derive from `ISystemCommand`.
     * @tparam M  The command map type that stores factory functions.
     * @param name  The textual name of the command (e.g., "in", "out").
     * @param commands  Reference to the module’s command factory map.
     */
     template <SystemCommandType T, CommandMapType M>
     constexpr void registerCommand(const std::string& name, M& commands) {
        if (commands.contains(name)) {
            LOG_WARN("[SystemModuleTemplate] Command '{}' already registered. Overwriting.", name);
        }
        commands[name] = [] { return std::make_unique<T>(); };
     }
 
 } 
 
 #endif
 