#include "Bitcode.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <platform/Triple.hpp>

namespace fs = std::filesystem;

namespace LynxPlatform {

    void Bitcode::print() const {
        std::cout << "[Bitcode]\n";
        std::cout << "  Type: " << static_cast<int>(m_type) << "\n";
        std::cout << "  Storage: " << (isEmbedded() ? "Embedded" : "FileSystem") << "\n";
        std::cout << "  OS: " << static_cast<int>(m_os) << "\n";
        std::cout << "  Arch: " << static_cast<int>(m_arch) << "\n";
        std::cout << "  Target: " << static_cast<int>(m_target) << "\n";
        if (!m_filename.empty()) std::cout << "  File: " << m_filename << "\n";
        if (m_lib && m_size) std::cout << "  Embedded size: " << m_size << " bytes\n";
    }

    bool Bitcode::fileExists() const {
        return m_storage == BitcodeStorage::FileSystem && fs::exists(m_filename);
    }

    llvm::Module *Bitcode::getLLVMModule() const {
        llvm::LLVMContext ctx;

        if (isEmbedded()) {
            auto buffer = llvm::MemoryBuffer::getMemBufferCopy(
                llvm::StringRef(reinterpret_cast<const char *>(m_lib), m_size), "<embedded>");
            auto moduleOrErr = parseBitcodeFile(buffer->getMemBufferRef(), ctx);

            if (!moduleOrErr) {
                logAllUnhandledErrors(moduleOrErr.takeError(), llvm::errs(), "Bitcode parse error: ");
                return nullptr;
            }
            return moduleOrErr->release();
        }

        if (isFromFile() && fileExists()) {
            auto bufferOrErr = llvm::MemoryBuffer::getFile(m_filename);
            if (!bufferOrErr) {
                llvm::errs() << "Failed to open bitcode file: " << m_filename << "\n";
                return nullptr;
            }

            auto moduleOrErr = parseBitcodeFile(bufferOrErr->get()->getMemBufferRef(), ctx);
            if (!moduleOrErr) {
                logAllUnhandledErrors(moduleOrErr.takeError(), llvm::errs(), "Bitcode parse error: ");
                return nullptr;
            }

            return moduleOrErr->release();
        }

        llvm::errs() << "Invalid Bitcode state\n";
        return nullptr;
    }

    Triple Bitcode::getTriple() const {
        return Triple(m_target, m_os, m_arch);
    }

    std::string Bitcode::getDescription() const {
        std::ostringstream oss;
        oss << "Bitcode[Type=" << static_cast<int>(m_type)
            << ", Storage=" << (isEmbedded() ? "Embedded" : "FileSystem")
            << ", OS=" << static_cast<int>(m_os)
            << ", Arch=" << static_cast<int>(m_arch)
            << ", Target=" << static_cast<int>(m_target);
        if (!m_filename.empty()) oss << ", File=" << m_filename;
        oss << "]";
        return oss.str();
    }

    llvm::Module *Bitcode::loadIfAvailable() const {
        // Can be extended to defer loading, if needed
        return getLLVMModule();
    }

}