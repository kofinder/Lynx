#ifndef LYNX_PLATFORM_BITCODE_HPP
#define LYNX_PLATFORM_BITCODE_HPP

#include <string>
#include <llvm/IR/Module.h>
#include <constants/ArchType.hpp>
#include <constants/TargetOSType.hpp>
#include <constants/LynxTargetType.hpp>

#include "Triple.hpp"

using namespace LynxConstants;

namespace LynxPlatform {

    class Bitcode {

        public:
            enum class BitcodeType { Dispatch, Builtins_c, LYNX_target, Stdlib };
            enum class BitcodeStorage { FileSystem, Embedded };

        private:
            BitcodeType m_type;
            BitcodeStorage m_storage;

            const unsigned char *m_lib = nullptr;
            size_t m_size = 0;

            TargetOSType m_os;
            ArchType m_arch;
            LynxTargetType m_target;

            std::string m_filename;

        public:
            // Dispatch constructor
            Bitcode(const unsigned char lib[], int size, TargetOSType os) {
                init(BitcodeType::Dispatch, BitcodeStorage::Embedded, lib, size, "", LynxTargetType::none, os, ArchType::error);
            }

            Bitcode(const char *filename, TargetOSType os) {
                init(BitcodeType::Dispatch, BitcodeStorage::FileSystem, nullptr, 0, filename, LynxTargetType::none, os, ArchType::error);
            }

            // Builtins-c constructor
            Bitcode(const unsigned char lib[], int size, TargetOSType os, ArchType arch) {
                init(BitcodeType::Builtins_c, BitcodeStorage::Embedded, lib, size, "", LynxTargetType::none, os, arch);
            }
            Bitcode(const char *filename, TargetOSType os, ArchType arch) {
                init(BitcodeType::Builtins_c, BitcodeStorage::FileSystem, nullptr, 0, filename, LynxTargetType::none, os, arch);
            }

            // Target constructor
            Bitcode(const unsigned char lib[], int size, LynxTargetType target, TargetOSType os, ArchType arch) {
                init(BitcodeType::LYNX_target, BitcodeStorage::Embedded, lib, size, "", target, os, arch);
            }

            Bitcode(const char *filename, LynxTargetType target, TargetOSType os, ArchType arch) {
                init(BitcodeType::LYNX_target, BitcodeStorage::FileSystem, nullptr, 0, filename, target, os, arch);
            }

            // General constructor
            Bitcode(BitcodeType type, const unsigned char lib[], int size, LynxTargetType target, TargetOSType os, ArchType arch) {
                init(type, BitcodeStorage::Embedded, lib, size, "", target, os, arch);
            }
            Bitcode(BitcodeType type, const char *filename, LynxTargetType target, TargetOSType os, ArchType arch) {
                init(type, BitcodeStorage::FileSystem, nullptr, 0, filename, target, os, arch);
            }

            void print() const;

            BitcodeType getType() const { return m_type; }
            BitcodeStorage getStorageType() const { return m_storage; }

            const unsigned char *getLib() const { return m_lib; }
            size_t getSize() const { return m_size; }

            TargetOSType getOS() const { return m_os; }
            ArchType getArch() const { return m_arch; }
            LynxTargetType getISPCTarget() const { return m_target; }

            const std::string &getFilename() const { return m_filename; }

            bool isEmbedded() const { return m_storage == BitcodeStorage::Embedded; }
            bool isFromFile() const { return m_storage == BitcodeStorage::FileSystem; }

            bool fileExists() const;

            llvm::Module *getLLVMModule() const;

            /// New: Return Triple object
            Triple getTriple() const;

            /// New: Human-friendly string
            std::string getDescription() const;

            /// Optional: stub for on-demand loading
            llvm::Module *loadIfAvailable() const;

        private:
            // Internal shared constructor
            void init(
                BitcodeType type, 
                BitcodeStorage storage, 
                const unsigned char *lib, 
                size_t size,
                const std::string &filename, 
                LynxTargetType target, 
                TargetOSType os, 
                ArchType arch) {
                m_type = type;
                m_storage = storage;
                m_lib = lib;
                m_size = size;
                m_filename = filename;
                m_target = target;
                m_os = os;
                m_arch = arch;
            }
    };
}

#endif 
