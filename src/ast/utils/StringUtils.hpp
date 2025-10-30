#ifndef LYNX_STRING_UTILS_HPP
#define LYNX_STRING_UTILS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <constants/Parameter.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <types/tmpl/TypeChecker.hpp>


namespace LynxAst::StringUtils {

    using namespace LynxConstants;
    using namespace LynxTypes;


    inline std::string encodeNameComponent(const std::string& name) {
        return std::to_string(name.length()) + name;
    }

    /**
     * @brief Represents a function or constructor signature in both readable and hashed form.
     * 
     * This structure stores a human-readable string representation of a function signature
     * (e.g., "int_string_double") as well as a hashed ID for fast equality comparison or lookup.
    */
    typedef struct { std::string readable;  size_t hashId; } SignatureInfo;

    inline std::string normalizeClassName(const std::string& className) {
        // Remove the LLVM prefix %class. if present
        const std::string prefix = "class.";
        if (className.rfind(prefix, 0) == 0) { // starts with "%class."
            return className.substr(prefix.size());
        }
        return className;
    }
    

    /**
     * @brief Generates a signature from a list of Parameter objects.
     * 
     * Iterates through the provided parameter list, converts each type to its string
     * representation, and concatenates them into a readable signature string separated by underscores.
     * Also computes a hash of the signature for quick lookups.
     * 
     * @param vecParams List of parameters to form the signature from.
     * @return SignatureInfo Struct containing the readable string and hash ID.
     */
    inline SignatureInfo sourceSignature(const std::string className, const std::vector<BaseType*> vecParams) {
        std::string sig = className + "_" + std::to_string(vecParams.size());

        for (const auto& param : vecParams) {
            if(auto clazzType = TypeCasting::castType<ClassType>(param)) { 
                sig += "C" + encodeNameComponent(clazzType->originalName());
            } else if(auto ifaceType = TypeCasting::castType<InterfaceType>(param)) {
                sig += "I" + encodeNameComponent(ifaceType->originalName());
            } else if(auto enumType = TypeCasting::castType<EnumType>(param)) {
                sig += "E" + encodeNameComponent(enumType->originalName());
            } else {
                sig += typeAbbrev(param->getTypeTag());
            }
        }

        size_t hashValue = std::hash<std::string>{}(sig);
        return { sig, hashValue };
    }

    /**
     * @brief Converts a string to lowercase.
     * 
     * Creates a lowercase copy of the provided string by transforming 
     * all alphabetic characters to their lowercase equivalents.
     * Non-alphabetic characters remain unchanged.
     * 
     * @param name Input string to transform.
     * @return std::string Lowercase version of the input string.
    */
    inline std::string transformLowerCase(const std::string& name) {
        std::string result = name;
        std::for_each(result.begin(), result.end(), [](char& c) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        });
        return result;
    }
    
    /**
     * @brief Replace path separators and dots with underscores.
     * 
     * Converts strings containing `/` or `.` into a form that is safe for 
     * use as identifiers or filenames by replacing them with `_`.
     * 
     * Example:
     * @code
     * sanitizeIdentifier("libs/document/file.lynx");
     * // returns "libs_document_file_lynx"
     * @endcode
     * 
     * @param name The input string to sanitize.
     * @return Sanitized string with '/' and '.' replaced by '_'.
     */
    inline std::string sanitizeIdentifier(const std::string& name) {
        std::string result = name;
        std::replace(result.begin(), result.end(), '/', '_');
        std::replace(result.begin(), result.end(), '.', '_');
        return result;
    }

    /**
     * @brief Resolve the absolute import path of a module, relative to the project root.
     * 
     * Searches all given `sourceFolders` for a `.lynx` file matching the given `moduleName`. 
     * If found, returns the path relative to the project root (``) without the `.lynx` extension.
     * 
     * This preserves the top-level folder name (e.g., `libs/` or `src/`) 
     * so that the returned path matches the import style in code.
     * 
     * Example:
     * @code
     * resolveImportPath(
     *     { "toy/libs", "toy/src" },
     *     "write_doc"
     * );
     * // returns "libs/document/interface/write_doc"
     * @endcode
     * 
     * @param sourceFolders List of source folder paths (absolute or relative to project root).
     * @param moduleName Name of the module to resolve (with or without `.lynx` extension).
     * @return Resolved import path without extension, relative to the project root. 
     *         If not found, returns the original moduleName.
     */
    inline std::string resolveImportPath(
        const std::vector<std::string>& sourceFolders,
        const std::string& moduleName
    ) {
        namespace fs = boost::filesystem;
        fs::path importPath(moduleName);
        if (importPath.extension().empty()) {
            importPath += ".lynx";
        }

        for (const auto& folder : sourceFolders) {
            if (!fs::exists(folder) || !fs::is_directory(folder)) {
                std::cerr << "[Warning] Folder not found or not a directory!: " << folder << std::endl;
                continue;
            }

            for (const auto& entry : fs::recursive_directory_iterator(folder)) {
                if (!fs::is_regular_file(entry) || entry.path().extension() != ".lynx") continue;

                // Compare filename only
                if (entry.path().filename() == importPath.filename()) {

                    // Make path relative to project root so "libs" is preserved
                    fs::path relativePath = fs::relative(entry.path(), "toy");

                    // Remove extension
                    relativePath = relativePath.parent_path() / relativePath.stem();

                    return relativePath.generic_string();
                }
            }
        }

        return moduleName;
    }


    /**
     * @brief Construct a human-readable function signature string from LLVM IR.
     *
     * This utility attempts to retrieve a function signature string from
     * LLVM metadata (`lynx.signature`). If the metadata exists and is a valid
     * `MDString`, it is returned directly. Otherwise, the function signature
     * is constructed manually using the provided return type name and parameter
     * list.
     *
     * @param llvmFunction   Reference to the LLVM function to inspect.
     * @param returnName     Name of the return type (e.g., "int").
     * @param fnParams       List of function parameters with type information.
     *
     * @return A string representing the function signature, e.g.:
     *         - From metadata: `"int (float, double)"`
     *         - From manual construction: `"int (i32, unknown)"`
     *
     * @note If parameter type information is missing or invalid, `"unknown"`
     *       will be used in its place.
    */
    inline std::string fnSignatureString(
        llvm::Function& llvmFunction,
        const std::string& funcName,
        const std::string& returnName,
        const std::vector<std::shared_ptr<Parameter>>& fnParams) 
    {
        // 1. Try to get signature from metadata
        if (auto* md = llvmFunction.getMetadata("lynx.signature")) {
            if (auto* node = llvm::dyn_cast<llvm::MDNode>(md)) {
                if (auto* str = llvm::dyn_cast<llvm::MDString>(node->getOperand(0))) {
                    return str->getString().str();
                }
            }
        }
    
        // 2. Fallback: manually construct signature
        std::ostringstream oss;
        oss << funcName << "(" << returnName ;

        for (size_t i = 0; i < fnParams.size(); ++i) {
           oss << ", ";
    
            const auto& param = fnParams[i];
            oss << (param && param->type && param->type->isValid()
                    ? param->type->name
                    : "unknown");
        }
    
        oss << ")";
        return oss.str();
    }
}

#endif
