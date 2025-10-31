#ifndef LYNX_CALLING_CONV_SPECIFICATION_HPP
#define LYNX_CALLING_CONV_SPECIFICATION_HPP

#include <llvm/IR/Function.h>
#include <logger/Logger.hpp>
#include <constants/CallingConventionType.hpp>

using namespace LynxConstants;
using namespace LynxLogger;

namespace LynxCallConv {

    /**
     * @brief Abstract base class representing a specification for a calling convention.
     * 
     * This interface is used to check whether a given LLVM function satisfies
     * a certain calling convention specification and to retrieve the corresponding
     * calling convention type.
    */
    class CallingConventionSpecification {

        public:

            /**
             * @brief Checks if the given function satisfies this calling convention specification.
             * 
             * @param func Pointer to the LLVM function to check.
             * @return true if the function satisfies the specification, false otherwise.
            */
            virtual bool isSatisfiedBy(llvm::Function* func) const = 0;

            /**
             * @brief Returns the calling convention type associated with this specification.
             * 
             * @return CallingConventionType enum value representing the calling convention.
            */
            virtual CallingConventionType getCallingConvention() const = 0;
            
            /**
             * @brief Virtual destructor for proper cleanup in derived classes.
            */
            virtual ~CallingConventionSpecification() = default;

        };
}

#endif 
