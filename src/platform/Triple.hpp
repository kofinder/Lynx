/**
 * @file Triple.hpp
 * @brief Represents a platform target triple for Lynx (target, OS, architecture, environment).
 * 
 * This class encapsulates the concept of a target triple, which is commonly used in compiler
 * toolchains to specify the combination of target platform, operating system, CPU architecture,
 * and optional environment. It provides utilities for encoding/decoding triples, converting to
 * LLVM triple strings, and creating Triple instances from strings.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

#ifndef LYNX_PLATFORM_TRIPLE_HPP
#define LYNX_PLATFORM_TRIPLE_HPP

#include <cstdint>
#include <constants/ArchType.hpp>
#include <constants/TargetOSType.hpp>
#include <constants/LynxTargetType.hpp>


namespace LynxPlatform {

    using namespace LynxConstants;

    class Triple {

        private:

            LynxTargetType targetType;

            TargetOSType osType;

            ArchType archType;

            std::string m_env;

        public:

            Triple(
                LynxTargetType target, 
                TargetOSType targetOS, 
                ArchType arch,
                const std::string& env = ""
            ) : targetType(target), osType(targetOS), archType(arch), m_env(env) {};

            /// @brief Returns the CPU architecture.
            ArchType getArch() const { return archType; }

            /// @brief Returns the target operating system.
            TargetOSType getOS() const { return osType; }

            /// @brief Returns the Lynx target type.
            LynxTargetType getTarget() const { return targetType; }

            /// @brief Returns the environment string.
            const std::string& getEnvironment() const { return m_env; }
        
            /**
             * @brief Converts the Triple to a human-readable string.
             * @return String representation of the triple.
             */
            std::string toString() const;

            /**
             * @brief Constructs a Triple from a string representation.
             * @param tripleStr String representing the triple.
             * @return Triple instance.
             */
            static Triple fromString(const std::string& tripleStr);
        
            /**
             * @brief Converts the Triple to an LLVM-compatible triple string.
             * @return LLVM triple string.
             */
            std::string toLLVMTriple() const;

            /**
             * @brief Constructs a Triple from an LLVM triple string.
             * @param llvmTripleStr LLVM triple string.
             * @return Triple instance.
             */
            static Triple fromLLVMTriple(const std::string& llvmTripleStr);

            /**
             * @brief Encodes the triple into a 32-bit integer representation for fast comparisons or lookups.
             * @return Encoded 32-bit integer.
             */
            uint32_t encode() const;
        
    };
    
}

#endif
