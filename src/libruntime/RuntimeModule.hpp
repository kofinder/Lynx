#ifndef LYNX_LIB_RUNTIME_MODULE_HPP
#define LYNX_LIB_RUNTIME_MODULE_HPP

#include <string>
#include <vector>
#include "RuntimeClassRegistry.hpp"
#include "RuntimeFunctionRegistry.hpp"

namespace LynxLibRuntime {

    /**
     * @class RuntimeModule
     * @brief Abstract base class for all runtime standard library modules.
     *
     * Each runtime module (e.g., Filesystem, Collections, Datetime) must inherit
     * from this interface and implement the required methods to register
     * its runtime functions and provide its identity.
     *
     * This class follows the Strategy and Interface patterns, enabling polymorphic
     * registration of runtime APIs.
     */
    class RuntimeModule {

        public:

            /**
             * @brief Registers this module’s class into the central function registry.
             * @param registry A reference to the RuntimeFunctionRegistry where functions are stored.
             */
            virtual void registerClasses(RuntimeClassRegistry& registry) = 0;
        
            /**
             * @brief Registers this module’s functions into the central function registry.
             * @param registry A reference to the RuntimeFunctionRegistry where functions are stored.
             */
            virtual void registerFunctions(RuntimeFunctionRegistry& registry) = 0;

            /**
             * @brief Returns the name of the module (e.g., "filesystem", "datetime").
             * @return The string identifier of the module.
            */
            virtual std::string name() const = 0;

            /**
             * @brief Virtual destructor to allow safe polymorphic cleanup.
            */
            virtual ~RuntimeModule() = default;
    };
}

#endif 
