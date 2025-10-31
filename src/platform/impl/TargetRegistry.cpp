#include "TargetRegistry.hpp"
#include <iostream>
#include <sstream>

namespace LynxPlatform {

    // Static member initialization
    std::vector<const Bitcode *> *TargetRegistry::libs = nullptr;

    // Private constructor
    TargetRegistry::TargetRegistry() 
        : m_dispatch(nullptr),
        m_dispatch_macos(nullptr),
        m_supported_oses(0) {
        if (!libs) {
            libs = new std::vector<const Bitcode *>();
        }
    }

    /**
     * @brief Registers a Bitcode library in the global registry.
     */
    void TargetRegistry::RegisterTarget(const Bitcode *lib) {
        if (!libs) {
            libs = new std::vector<const Bitcode *>();
        }
        libs->push_back(lib);
    }

    /**
     * @brief Returns the singleton instance of the target registry.
     */
    TargetRegistry *TargetRegistry::getTargetLibRegistry() {
        static TargetRegistry instance;

        if (libs && !libs->empty()) {
            for (const Bitcode *lib : *libs) {
                // Categorize and store Bitcode libraries by type and attributes
                switch (lib->getType()) {
                    case Bitcode::BitcodeType::Dispatch:
                        if (lib->getOS() == TargetOSType::macos)
                            instance.m_dispatch_macos = lib;
                        else
                            instance.m_dispatch = lib;
                        break;
                    case Bitcode::BitcodeType::Builtins_c:
                        instance.m_builtins[(static_cast<uint32_t>(lib->getOS()) << 16) | static_cast<uint32_t>(lib->getArch())] = lib;
                        break;
                    case Bitcode::BitcodeType::LYNX_target:
                        instance.m_targets[(static_cast<uint32_t>(lib->getISPCTarget()) << 24) |
                                        (static_cast<uint32_t>(lib->getOS()) << 16) |
                                        static_cast<uint32_t>(lib->getArch())] = lib;
                        instance.m_supported_oses.set(static_cast<int>(lib->getOS()));
                        break;
                    case Bitcode::BitcodeType::Stdlib:
                        instance.m_stdlibs[(static_cast<uint32_t>(lib->getISPCTarget()) << 24) |
                                        (static_cast<uint32_t>(lib->getOS()) << 16) |
                                        static_cast<uint32_t>(lib->getArch())] = lib;
                        instance.m_supported_oses.set(static_cast<int>(lib->getOS()));
                        break;
                    default:
                        break;
                }
            }
        }
        return &instance;
    }

    /**
     * @brief Returns the dispatch library for a specified OS.
     */
    const Bitcode *TargetRegistry::getDispatchLibrary(const TargetOSType os) const {
        if (os == TargetOSType::macos && m_dispatch_macos)
            return m_dispatch_macos;
        return m_dispatch;
    }

    /**
     * @brief Returns the builtins-c library for a specific OS and architecture.
     */
    const Bitcode *TargetRegistry::getBuiltinsCLibray(TargetOSType os, ArchType arch) const {
        uint32_t key = (static_cast<uint32_t>(os) << 16) | static_cast<uint32_t>(arch);
        auto it = m_builtins.find(key);
        if (it != m_builtins.end()) return it->second;
        return nullptr;
    }

    /**
     * @brief Returns the ISPC target library for the given target, OS, and architecture.
     */
    const Bitcode *TargetRegistry::getTargetLibray(LynxTargetType target, TargetOSType os, ArchType arch) const {
        uint32_t key = (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(os) << 16) | static_cast<uint32_t>(arch);
        auto it = m_targets.find(key);
        if (it != m_targets.end()) return it->second;
        return nullptr;
    }

    /**
     * @brief Returns the ISPC stdlib library for the given target, OS, and architecture.
     */
    const Bitcode *TargetRegistry::getStdLibray(LynxTargetType target, TargetOSType os, ArchType arch) const {
        uint32_t key = (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(os) << 16) | static_cast<uint32_t>(arch);
        auto it = m_stdlibs.find(key);
        if (it != m_stdlibs.end()) return it->second;
        return nullptr;
    }

    /**
     * @brief Checks for missing bitcode libraries and returns their filenames.
     */
    std::vector<std::string> TargetRegistry::checkBitcodeLibs() const {
        std::vector<std::string> missing;

        // Check dispatch
        if (!m_dispatch) missing.push_back("Dispatch bitcode");
        if (!m_dispatch_macos) missing.push_back("Dispatch macOS bitcode");

        // Check builtins
        for (const auto& kv : m_builtins) {
            if (!kv.second) missing.push_back("Builtins-c bitcode for key " + std::to_string(kv.first));
        }

        // Check targets
        for (const auto& kv : m_targets) {
            if (!kv.second) missing.push_back("Lynx target bitcode for key " + std::to_string(kv.first));
        }

        // Check stdlibs
        for (const auto& kv : m_stdlibs) {
            if (!kv.second) missing.push_back("Stdlib bitcode for key " + std::to_string(kv.first));
        }

        return missing;
    }

    /**
     * @brief Prints a matrix of supported targets, OSes, and architectures.
     */
    void TargetRegistry::printSupportMatrix() const {
        // std::cout << "Supported Operating Systems: " << getSupportedOSes() << "\n";
        // std::cout << "Supported Architectures: " << getSupportedArchTypes() << "\n";
        // std::cout << "Supported Targets: " << getSupportedTargets() << "\n";

        auto missing = checkBitcodeLibs();
        if (!missing.empty()) {
            std::cout << "Warning: Missing bitcode libraries:\n";
            for (const auto& m : missing)
                std::cout << " - " << m << "\n";
        }
    }

    /**
     * @brief Returns a comma-separated string listing supported architecture types.
     */
    std::string TargetRegistry::getSupportedArchTypes() {
        std::set<ArchType> archs;
        for (const auto& kv : m_builtins)
            archs.insert(static_cast<ArchType>(kv.first & 0xFFFF));
        std::stringstream ss;
        bool first = true;
        for (auto a : archs) {
            if (!first) ss << ", ";
            ss << static_cast<int>(a); // Replace with name if available
            first = false;
        }
        return ss.str();
    }

    /**
     * @brief Returns a comma-separated string listing supported Lynx targets.
     */
    std::string TargetRegistry::getSupportedTargets() {
        std::set<LynxTargetType> targets;
        for (const auto& kv : m_targets)
            targets.insert(static_cast<LynxTargetType>((kv.first >> 24) & 0xFF));
        std::stringstream ss;
        bool first = true;
        for (auto t : targets) {
            if (!first) ss << ", ";
            ss << static_cast<int>(t); // Replace with name if available
            first = false;
        }
        return ss.str();
    }

    /**
     * @brief Returns a comma-separated string listing supported operating systems.
     */
    std::string TargetRegistry::getSupportedOSes() {
        std::stringstream ss;
        bool first = true;
        for (int i = 0; i < (int)TargetOSType::error; ++i) {
            if (m_supported_oses.test(i)) {
                if (!first) ss << ", ";
                ss << i; // Replace with name if available
                first = false;
            }
        }
        return ss.str();
    }

    /**
     * @brief Determines whether a combination of target, OS, and architecture is supported.
     */
    bool TargetRegistry::isSupported(LynxTargetType target, TargetOSType os, ArchType arch) const {
        uint32_t key = (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(os) << 16) | static_cast<uint32_t>(arch);
        if (m_targets.find(key) != m_targets.end()) return true;
        if (m_stdlibs.find(key) != m_stdlibs.end()) return true;
        if (m_builtins.find((static_cast<uint32_t>(os) << 16) | static_cast<uint32_t>(arch)) != m_builtins.end()) return true;
        return false;
    }

}
