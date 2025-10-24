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
    
    /**
     * @brief Core engine for applying different name mangling strategies.
     *
     * The ManglingEngine uses the Strategy design pattern via the NameManglingContext
     * to apply a specific mangling algorithm depending on the provided ManglerType.
     * 
     * Supported mangling types include Itanium (GCC), Microsoft, and a custom Lynx format.
     * Provides a clean dispatch system for different mangling operations via function mapping.
    */
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
