/**
 * @file Bitcode.hpp
 * @brief Represents a Lynx platform bitcode library and its associated metadata.
 *
 * This class encapsulates different types of bitcode libraries (Dispatch, Builtins-c,
 * target-specific, and Stdlib) and provides methods for querying their storage, location,
 * and target platform information.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

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
 
     /**
      * @class Bitcode
      * @brief Represents a bitcode library and its platform metadata.
      *
      * Supports multiple storage types (Embedded, FileSystem) and bitcode categories:
      * Dispatch, Builtins-c, LYNX_target, and Stdlib.
      */
     class Bitcode {
 
         public:
             /// Bitcode category
             enum class BitcodeType { Dispatch, Builtins_c, LYNX_target, Stdlib };
 
             /// Storage location of the bitcode
             enum class BitcodeStorage { FileSystem, Embedded };
 
         private:
             BitcodeType m_type;           ///< Type of the bitcode
             BitcodeStorage m_storage;     ///< Storage type (embedded or file)
 
             const unsigned char *m_lib = nullptr; ///< Pointer to embedded bitcode
             size_t m_size = 0;                    ///< Size of the embedded bitcode
 
             TargetOSType m_os;           ///< Target operating system
             ArchType m_arch;             ///< Target CPU architecture
             LynxTargetType m_target;     ///< Lynx target type
 
             std::string m_filename;      ///< Path to file (if stored on filesystem)
 
         public:
             // Dispatch constructors
             Bitcode(const unsigned char lib[], int size, TargetOSType os);
             Bitcode(const char *filename, TargetOSType os);
 
             // Builtins-c constructors
             Bitcode(const unsigned char lib[], int size, TargetOSType os, ArchType arch);
             Bitcode(const char *filename, TargetOSType os, ArchType arch);
 
             // Target-specific constructors
             Bitcode(const unsigned char lib[], int size, LynxTargetType target, TargetOSType os, ArchType arch);
             Bitcode(const char *filename, LynxTargetType target, TargetOSType os, ArchType arch);
 
             // General constructor
             Bitcode(BitcodeType type, const unsigned char lib[], int size, LynxTargetType target, TargetOSType os, ArchType arch);
             Bitcode(BitcodeType type, const char *filename, LynxTargetType target, TargetOSType os, ArchType arch);
 
             /// Print a description of the bitcode
             void print() const;
 
             /// Getters
             BitcodeType getType() const { return m_type; }
             BitcodeStorage getStorageType() const { return m_storage; }
             const unsigned char *getLib() const { return m_lib; }
             size_t getSize() const { return m_size; }
             TargetOSType getOS() const { return m_os; }
             ArchType getArch() const { return m_arch; }
             LynxTargetType getLynxTarget() const { return m_target; }
             const std::string &getFilename() const { return m_filename; }
 
             /// Returns true if bitcode is embedded
             bool isEmbedded() const { return m_storage == BitcodeStorage::Embedded; }
 
             /// Returns true if bitcode is stored on filesystem
             bool isFromFile() const { return m_storage == BitcodeStorage::FileSystem; }
 
             /// Checks whether the file exists (for FileSystem storage)
             bool fileExists() const;
 
             /// Returns the LLVM Module associated with the bitcode
             llvm::Module *getLLVMModule() const;
 
             /// Returns the Triple object for this bitcode
             Triple getTriple() const;
 
             /// Returns a human-readable description
             std::string getDescription() const;
 
             /// Optional: loads the LLVM module on demand
             llvm::Module *loadIfAvailable() const;
 
         private:
             /// Shared initializer for all constructors
             void init(
                 BitcodeType type, 
                 BitcodeStorage storage, 
                 const unsigned char *lib, 
                 size_t size,
                 const std::string &filename, 
                 LynxTargetType target, 
                 TargetOSType os, 
                 ArchType arch
             ) {
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
 