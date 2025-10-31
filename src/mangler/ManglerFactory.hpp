#ifndef LYNX_MANGLER_FACTORY_HPP
#define LYNX_MANGLER_FACTORY_HPP

#include <memory>
#include "IMangleStrategy.hpp"
#include "kinds/CustomMangler.hpp"
#include "kinds/ItaniumMangler.hpp"
#include "kinds/MicrosoftMangler.hpp"
#include <constants/ManglerType.hpp>

using namespace LynxConstants;

namespace LynxMangler {

    /**
     * @brief Factory class to create instances of IMangleStrategy based on the specified ManglerType.
     * 
     * This class provides a static interface to instantiate different mangling strategies
     * (e.g., Itanium, Microsoft, or custom manglers). Extend this factory to support new mangling styles.
    */
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
