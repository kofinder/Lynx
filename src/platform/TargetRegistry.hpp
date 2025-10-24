#ifndef LYNX_PLATFORM_TARGET_REGISTRY_HPP
#define LYNX_PLATFORM_TARGET_REGISTRY_HPP

#include <string>
#include <map>
#include <vector>
#include <bitset>
#include <llvm/IR/Module.h>
#include <constants/ArchType.hpp>
#include <constants/TargetOSType.hpp>
#include <constants/LynxTargetType.hpp>

#include "Triple.hpp"
#include "Bitcode.hpp"


namespace LynxPlatform {
    
    class TargetRegistry {
        
        static std::vector<const Bitcode *> *libs;

        TargetRegistry();

        // Dispatch libraries

        /**
         * @brief Dispatch module for general platforms.
         */
        const Bitcode *m_dispatch;

        /**
         * @brief Dispatch module specifically for macOS.
         */
        const Bitcode *m_dispatch_macos;

        // Builtins-c libraries keyed by OS x ArchType

        /**
         * @brief Map of builtins-c bitcode libraries keyed by OS and architecture.
         */
        std::map<uint32_t, const Bitcode *> m_builtins;

        // ISPC target libraries keyed by Target x OS x ArchType

        /**
         * @brief Map of ISPC target bitcode libraries keyed by target, OS, and architecture.
         */
        std::map<uint32_t, const Bitcode *> m_targets;

        // Stdlib libraries keyed by Target x OS x ArchType

        /**
         * @brief Map of stdlib bitcode libraries keyed by target, OS, and architecture.
         */
        std::map<uint32_t, const Bitcode *> m_stdlibs;

        /**
         * @brief Bitset tracking supported operating systems.
         */
        std::bitset<(int)TargetOSType::error> m_supported_oses;

    public:
        /**
         * @brief Registers a Bitcode library in the global registry.
         * 
         * @param lib Pointer to the Bitcode to register.
         */
        static void RegisterTarget(const Bitcode *lib);

        /**
         * @brief Returns the singleton instance of the target registry.
         * 
         * @return Pointer to the global TargetRegistry instance.
         */
        static TargetRegistry *getTargetLibRegistry();

        /**
         * @brief Returns the dispatch library for a specified OS.
         * 
         * @param os Target operating system.
         * @return Pointer to the dispatch Bitcode if available, nullptr otherwise.
         */
        const Bitcode *getDispatchLibrary(const TargetOSType os) const;

        /**
         * @brief Returns the builtins-c library for a specific OS and architecture.
         * 
         * @param os Target operating system.
         * @param arch Target architecture.
         * @return Pointer to the builtins-c Bitcode if available, nullptr otherwise.
         */
        const Bitcode *getBuiltinsCLibray(TargetOSType os, ArchType arch) const;

        /**
         * @brief Returns the ISPC target library for the given target, OS, and architecture.
         * 
         * @param target Lynx target type.
         * @param os Target operating system.
         * @param arch Target architecture.
         * @return Pointer to the ISPC target Bitcode if available, nullptr otherwise.
         */
        const Bitcode *getTargetLibray(LynxTargetType target, TargetOSType os, ArchType arch) const;

        /**
         * @brief Returns the ISPC stdlib library for the given target, OS, and architecture.
         * 
         * @param target Lynx target type.
         * @param os Target operating system.
         * @param arch Target architecture.
         * @return Pointer to the ISPC stdlib Bitcode if available, nullptr otherwise.
         */
        const Bitcode *getStdLibray(LynxTargetType target, TargetOSType os, ArchType arch) const;

        /**
         * @brief Checks for missing bitcode libraries and returns their filenames.
         * 
         * @return Vector of filenames corresponding to missing bitcode libraries.
         */
        std::vector<std::string> checkBitcodeLibs() const;

        /**
         * @brief Prints a matrix of supported targets, OSes, and architectures.
         */
        void printSupportMatrix() const;

        /**
         * @brief Returns a comma-separated string listing supported architecture types.
         * 
         * @return String containing supported architecture names.
         */
        std::string getSupportedArchTypes();

        /**
         * @brief Returns a comma-separated string listing supported Lynx targets.
         * 
         * @return String containing supported Lynx target names.
         */
        std::string getSupportedTargets();

        /**
         * @brief Returns a comma-separated string listing supported operating systems.
         * 
         * @return String containing supported OS names.
         */
        std::string getSupportedOSes();

        /**
         * @brief Determines whether a combination of target, OS, and architecture is supported.
         * 
         * @param target Lynx target type.
         * @param os Target operating system.
         * @param arch Target architecture.
         * @return True if the combination is supported, false otherwise.
         */
        bool isSupported(LynxTargetType target, TargetOSType os, ArchType arch) const;
    };
}

#endif
