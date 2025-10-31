#ifndef LYNX_IJIT_HPP
#define LYNX_IJIT_HPP

#include <memory>
#include <llvm/IR/Module.h>

namespace LynxJIT {

    /**
     * @brief Interface for all JIT (Just-In-Time) execution engines.
     *
     * This abstract class defines the common interface that different
     * JIT backends (e.g., MCJIT, ORC/LLJIT) must implement. It allows
     * a unified way of initializing and executing compiled LLVM IR modules.
    */
    class IJIT {

        public:

            /**
             * @brief Initializes the JIT with a given LLVM IR module.
             *
             * This method sets up the JIT engine by taking ownership of an LLVM Module.
             * Derived classes must implement their own way of configuring and preparing
             * the module for execution (e.g., MCJIT or ORC JIT setup).
             *
             * @param module A unique pointer to an LLVM IR Module.
            */
            virtual void initialize(std::unique_ptr<llvm::Module> module) = 0;

            /**
             * @brief Executes the compiled 'main' function in the module.
             *
             * This function runs the entry point (typically "main") of the compiled module.
             * It must be implemented by each JIT engine based on how it handles symbol
             * resolution and function invocation.
             *
             * @return The exit code or return value from the 'main' function.
            */
            virtual int execute() = 0;

            /**
             * @brief Virtual destructor for safe cleanup of derived classes.
            */
            virtual ~IJIT() = default;
    };

}

#endif
