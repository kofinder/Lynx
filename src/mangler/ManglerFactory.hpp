/**
 * @file ManglerFactory.hpp
 * @brief Provides a factory for creating name mangling strategy instances.
 *
 * This header defines the `ManglerFactory` class, which generates instances of 
 * classes implementing the `IMangleStrategy` interface. The factory allows selection 
 * of different mangling schemes, including Itanium, Microsoft, and a custom Lynx mangler.
 *
 * **Key Features:**
 * - Centralized creation of mangler strategies based on `ManglerType`.
 * - Returns a `std::unique_ptr<IMangleStrategy>` to the selected mangler.
 * - Throws exceptions for unknown or unsupported mangler types.
 *
 * Usage Example:
 * @code
 * auto mangler = ManglerFactory::create(ManglerType::ITANIUM);
 * std::string mangledName = mangler->mangleFunction("foo", params);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_MANGLER_FACTORY_HPP
#define LYNX_MANGLER_FACTORY_HPP

#include <memory>
#include "IMangleStrategy.hpp"
#include "kinds/CustomMangler.hpp"
#include "kinds/ItaniumMangler.hpp"
#include "kinds/MicrosoftMangler.hpp"
#include <constants/ManglerType.hpp>


namespace LynxMangler {

    using namespace LynxConstants;

    class ManglerFactory {

        public:

            /**
             * @brief Creates an instance of a mangling strategy based on the given ManglerType.
             * 
             * @param type The type of mangler to create (e.g., Itanium, Microsoft, Custom).
             * @return A unique pointer to an instance of a class implementing IMangleStrategy.
             * 
             * @throws std::runtime_error If the specified mangler is not implemented.
             * @throws std::invalid_argument If the ManglerType is unknown.
             */
            static std::unique_ptr<IMangleStrategy> create(ManglerType type) {
                switch (type) {
                    case ManglerType::ITANIUM:      return std::make_unique<ItaniumMangler>();
                    case ManglerType::MICROSOFT:    return std::make_unique<MicrosoftMangler>();
                    case ManglerType::LYNX:         return std::make_unique<CustomMangler>();
                    default:                        return std::make_unique<ItaniumMangler>();
                }

            }
    };
}

#endif
