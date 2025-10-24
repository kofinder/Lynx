#include "Triple.hpp"
#include <llvm/ADT/Triple.h>
#include <sstream>

namespace LynxPlatform {

    std::string Triple::toString() const {
        return parseArchTypeToString(archType) + "-" +
            parseStringToOSType(osType) + "-" +
            parseTargetTypeToString(targetType) +
            (m_env.empty() ? "" : "-" + m_env);
    }

    std::string Triple::toLLVMTriple() const {
        llvm::Triple triple;

        // Arch
        switch (archType) {
            case ArchType::x86:      triple.setArch(llvm::Triple::x86); break;
            case ArchType::x86_64:   triple.setArch(llvm::Triple::x86_64); break;
            case ArchType::arm:      triple.setArch(llvm::Triple::arm); break;
            case ArchType::aarch64:  triple.setArch(llvm::Triple::aarch64); break;
            case ArchType::wasm32:   triple.setArch(llvm::Triple::wasm32); break;
            case ArchType::wasm64:   triple.setArch(llvm::Triple::wasm64); break;
            default:                 triple.setArch(llvm::Triple::UnknownArch); break;
        }

        // OS
        switch (osType) {
            case TargetOSType::linux:     triple.setOS(llvm::Triple::Linux); break;
            case TargetOSType::windows:   triple.setOS(llvm::Triple::Win32); break;
            case TargetOSType::macos:     triple.setOS(llvm::Triple::MacOSX); break;
            case TargetOSType::android:   triple.setOS(llvm::Triple::Linux); break;
            case TargetOSType::ios:       triple.setOS(llvm::Triple::IOS); break;
            case TargetOSType::web:       triple.setOS(llvm::Triple::WASI); break;
            default:                      triple.setOS(llvm::Triple::UnknownOS); break;
        }

        if (!m_env.empty()) {
            triple.setEnvironmentName(m_env);
        }

        return triple.str();
    }

    Triple Triple::fromLLVMTriple(const std::string& llvmTripleStr) {
        llvm::Triple llvmTriple(llvmTripleStr);
        ArchType arch = ArchType::error;
        TargetOSType os = TargetOSType::error;

        // Arch
        switch (llvmTriple.getArch()) {
            case llvm::Triple::x86:      arch = ArchType::x86; break;
            case llvm::Triple::x86_64:   arch = ArchType::x86_64; break;
            case llvm::Triple::arm:      arch = ArchType::arm; break;
            case llvm::Triple::aarch64:  arch = ArchType::aarch64; break;
            case llvm::Triple::wasm32:   arch = ArchType::wasm32; break;
            case llvm::Triple::wasm64:   arch = ArchType::wasm64; break;
            default:                     arch = ArchType::error; break;
        }

        // OS
        switch (llvmTriple.getOS()) {
            case llvm::Triple::Linux:       os = TargetOSType::linux; break;
            case llvm::Triple::Win32:       os = TargetOSType::windows; break;
            case llvm::Triple::MacOSX:      os = TargetOSType::macos; break;
            // case llvm::Triple::Android:     os = TargetOSType::android; break;
            case llvm::Triple::IOS:         os = TargetOSType::ios; break;
            case llvm::Triple::WASI:        os = TargetOSType::web; break;
            default:                        os = TargetOSType::error; break;
        }

        return Triple(LynxTargetType::host, os, arch, llvmTriple.getEnvironmentName().str());
    }

    uint32_t Triple::encode() const {
        return (static_cast<uint32_t>(archType) << 16) |
            (static_cast<uint32_t>(osType) << 8) |
            (static_cast<uint32_t>(targetType));
    }

} // namespace LynxPlatform
