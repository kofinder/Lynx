/**
 * @file ManglingEngine.hpp
 * @brief Provides a high-level interface for name mangling using configurable strategies.
 *
 * The `ManglingEngine` class encapsulates a `ManglingContext` and simplifies the process
 * of mangling function names, member functions, constructors, and class names. It allows
 * selecting a mangling strategy (Itanium, Microsoft, or Lynx custom) at construction.
 *
 * **Key Features:**
 * - Initializes the mangling context with a chosen `ManglerType`.
 * - Delegates mangling requests to the `ManglingContext`.
 * - Provides `mangle()` for mangling names based on `MangleParameter`.
 * - Move-only semantics: copy operations are deleted, move operations are defaulted.
 * - Throws exceptions if an unsupported mangler type is requested or if required parameters
 *   are missing for the mangler kind.
 *
 * Usage Example:
 * @code
 * LynxMangler::ManglingEngine engine(LynxConstants::ManglerType::ITANIUM);
 * std::string mangledName = engine.mangle(
 *     LynxMangler::MangleParameter::makeFunction("foo", params)
 * );
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_MANGLING_ENGINE_HPP
#define LYNX_MANGLING_ENGINE_HPP

#include <memory>
#include <unordered_map>
#include <functional>
#include <stdexcept>

#include "ManglingContext.hpp"
#include "ManglerFactory.hpp"
#include "MangleParamter.hpp"
#include <constants/ManglerType.hpp>

namespace LynxMangler {
    
    class ManglingEngine {

        private: 
        
            ManglingContext ctx;

        public:

            /**
             * @brief Constructs the engine with the given mangler type.
             * 
             * @param type The ManglerType (Itanium, Microsoft, or Lynx).
             * @throws std::invalid_argument or std::runtime_error if the type is not implemented.
            */
            explicit ManglingEngine(ManglerType type) {
                ctx.setStrategy(ManglerFactory::create(type));
            }

            std::string mangle(const MangleParameter& param) {
                return ctx.delegateMangle(param);
            } 

            ManglingEngine(const ManglingEngine&) = delete;
            ManglingEngine& operator=(const ManglingEngine&) = delete;
    
            ManglingEngine(ManglingEngine&&) noexcept = default;
            ManglingEngine& operator=(ManglingEngine&&) noexcept = default;
    };
}

#endif
