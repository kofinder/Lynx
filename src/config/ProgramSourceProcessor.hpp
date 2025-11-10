/**
 * @file ProgramOptionConfig.hpp
 * @brief Declares the ProgramOptionConfig class for centralized CLI option management in the Lynx compiler.
 * 
 * The ProgramOptionConfig class provides a singleton-based interface to parse, store, and access
 * command-line arguments and program options for Lynx applications. It leverages Boost.Program_options
 * to support a variety of commands such as `run`, `build`, `analyze`, `create`, `clean`, `test`,
 * `upgrade`, `downgrade`, and `help`.
 * 
 * **Key Responsibilities:**
 * - Parse and validate CLI arguments and options.
 * - Provide accessors for commonly used program paths, entry files, build directories, and source folders.
 * - Track flags such as debug, dry-run, verbosity, syntax highlighting, and source emission.
 * - Provide utility methods to print banners, usage, and help information.
 * - Load additional configuration from YAML files for project-specific settings.
 * 
 * **Used By:**
 * - The main Lynx program entry point to determine the execution workflow.
 * - Build, run, and analysis subsystems to configure program behavior based on CLI options.
 * - Utility scripts and tools interacting with the Lynx compiler framework.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_PROGRAM_SOURCE_PROCESSOR_HPP
#define LYNX_PROGRAM_SOURCE_PROCESSOR_HPP

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <ast/Node.hpp>
#include <lang/LynxDriver.hpp>

using namespace LynxLang;
using namespace LynxAst;
using namespace LynxLogger;
using namespace LynxContext;

namespace fs = boost::filesystem;

namespace LynxProgramConfig {

    class ProgramSourceProcessor {

        private:

            int exitCode;

            std::string buildDir;

            std::string entryFile;

            std::unique_ptr<LynxDriver> driver; 

            std::vector<std::string> sourceFolders;

            std::unordered_set<std::string> parsedFiles;

            std::unordered_map<std::string, std::unique_ptr<Node>> moduleAstMap;

            /**
             * @brief Set the input stream and its name.
             *
             * @param stream Reference to the input stream.
             * @param name Name of the input stream for identification.
             */
            void setInputStream(std::istream& stream, const std::string& name);

            /**
             * @brief Set the file stream based on the provided filename.
             *
             * @param fileName Reference to the filename string.
             */
            void setFileStream(const std::string& fileName);

             /**
             * @brief Derives a module name from the given file path, relative to source folders.
             *
             * @param filePath Full path to the source file.
             * @param sourceFolders List of root source directories to resolve the module name.
             * @return Relative module name string.
             */
            std::string deriveModuleName(const std::string& filePath, const std::vector<std::string>& sourceFolders);

            /**
             * @brief Recursively parse a single Lynx source file and its imports.
             *
             * This function is responsible for parsing a given `.lynx` source file,
             * resolving its import dependencies, and parsing those as well.
             * It ensures that each file is only parsed once by tracking processed files
             * internally (typically using a set or map).
             *
             * This enables modular parsing of large projects with multiple files
             * and import statements.
             *
             * @param filePath The absolute or relative path to the Lynx source file.
             */
            void parseFileRecursive(const std::string& filePath);


        public:

            ProgramSourceProcessor() : exitCode(0) {
                driver = std::make_unique<LynxDriver>();
            }

            /**
             * @brief Load all relevant source files for the project, including the entry file and source folders.
             *
             * This function is responsible for initializing the compilation unit by:
             *   - Parsing the main entry file specified in the project configuration.
             *   - Recursively scanning and loading all `.lynx` source files from the given folders.
             *   - Resolving and parsing imports found within each file.
             * 
             * This method ensures that all project-relevant files are parsed and prepared for
             * further processing such as semantic analysis, code generation, or execution.
             *
             * @param buildDir The file path to the bild of the program (usually `main.lynx`).
             * @param entrySource The file path to the entry point of the program (usually `main.lynx`).
             * @param sourceFolders A list of directory paths containing additional source files.
            */
            void load(const std::string& buildDir, const std::string& entrySource, const std::vector<std::string>& sourceFolders);

            /**
             * @brief Parses the entry file and all related project source files.
             *
             * This method reads and parses the entry `.lynx` file along with all additional
             * source files specified in the project folders. The parsing results in an
             * abstract syntax tree (AST) representing the program structure.
             *
             * This function only performs syntax parsing. Code generation and execution
             * should be handled separately (e.g., via generateAbstractSyntax() and executeJIT()).
             *
             * Use this method after calling `load()` to process the source tree.
             */
            void parseSources();

            /**
             * @brief Parse the input stream and generate an AST.
             *
             * This function processes the input stream and constructs the corresponding
             * abstract syntax tree for further operations.
             */
            void parseStream(const std::string& streamName);

            /**
             * @brief Retrieve the exit code of the Lynx execution.
             *
             * This method returns the exit code set during the execution of the Lynx
             * interpreter or JIT compilation. A zero exit code typically indicates success,
             * while a non-zero value may indicate an error or specific exit condition.
             *
             * @return The exit code for the Lynx execution.
             */
            int getExitCode() const;

            
            /**
             * @brief Execute the Just-In-Time (JIT) compilation of the Lynx code.
             *
             * This method compiles the Lynx language code to native machine code
             * using JIT compilation techniques, allowing for execution of the
             * code directly in memory. It handles the necessary setup and 
             * execution flow to run the compiled code.
             */
            // void executeJIT();

            /**
             * @brief Provides read-only access to the module-to-AST map.
             * 
             * This accessor returns a const reference to the internal `moduleAstMap`,
             * which holds the parsed abstract syntax tree (AST) for each source module.
             * Used primarily by downstream components like the IR generator to traverse
             * and analyze ASTs without taking ownership.
             * 
             * @return A const reference to the map of module names to their root AST nodes.
             */
            const std::unordered_map<std::string, std::unique_ptr<Node>>& getModuleAstMap() const { return moduleAstMap; }

            /**
             * @brief Transfers ownership of the internal LynxDriver instance.
             * 
             * This function returns a `std::unique_ptr` to the internal `LynxDriver`,
             * allowing the caller to take exclusive ownership. After calling this,
             * the driver within the processor becomes null and cannot be used again
             * unless reinitialized. Useful for passing the driver into components like
             * the IR generator which manage their own lifecycle.
             * 
             * @return A unique pointer to the owned LynxDriver instance.
             */
            std::unique_ptr<LynxDriver> getDriver() { return std::move(driver); }

            /**
             * @brief Retrieves a reference to the list of user-defined classes.
             * 
             * Provides direct access to the vector storing class names defined
             * by the user during compilation.
             * 
             * @return Reference to a vector of user-defined class names.
            */
            std::vector<std::string>& getUserDefinedClasses();

            ~ProgramSourceProcessor() = default;

    };
}
#endif
