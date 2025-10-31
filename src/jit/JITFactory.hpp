#ifndef LYNX_JIT_FACTORY_HPP
#define LYNX_JIT_FACTORY_HPP

#include "IJIT.hpp"
#include "MCJITEngine.hpp"
#include "ORCJITEngine.hpp"
#include "JITConstants.hpp"

namespace LynxJIT {

    /**
     * @brief Factory class to create instances of different JIT engine implementations.
     *
     * This class uses the Factory Design Pattern to abstract the construction of
     * specific JIT engines like MCJIT or ORCJIT. Clients can request a desired
     * engine type without needing to know the concrete implementation details.
    */
    class JITFactory {

        public:

            /**
             * @brief Creates an instance of a JIT engine based on the requested type.
             *
             * @param type An enum value of type JITType indicating the desired JIT backend.
             * @return A unique pointer to the created IJIT implementation (e.g., MCJITEngine or ORCJITEngine).
             *
             * @throws std::runtime_error if the provided JITType is unsupported.
            */
            static std::unique_ptr<IJIT> create(JITType type) {
                switch (type) {
                    case JITType::MCJIT:
                        return std::make_unique<MCJITEngine>();
                    case JITType::ORCJIT:
                        return std::make_unique<ORCJITEngine>();
                    default:
                        throw std::runtime_error("Unsupported JITType.");
                }            
            }
    };
}

#endif 
