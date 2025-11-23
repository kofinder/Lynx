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
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_PROGRAM_OPTION_CONFIG_HPP
#define LYNX_PROGRAM_OPTION_CONFIG_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options/errors.hpp>
#include "ProgramSourceProcessor.hpp"

namespace LynxProgramConfig {

    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

    class ProgramOptionConfig {

        private:

            std::string cmd;

            po::variables_map vm;
            
            static bool is_initialized;

        public:
        
            std::string rootPath;
            
            std::string buildDir;

            std::string configFile;

            std::string entrySource;

            std::vector<std::string> sourceFolders;

            /**
             * @brief Default constructor & Destructor
             */
            ProgramOptionConfig() noexcept = default;
            ~ProgramOptionConfig() noexcept = default;

            // Delete copy and move operations for singleton enforcement
            ProgramOptionConfig(const ProgramOptionConfig&) = delete;
            ProgramOptionConfig& operator=(const ProgramOptionConfig&) = delete;

            /**
             * @brief Access the singleton instance of the option config.
             * 
             * @return Reference to the singleton object.
             */
            static ProgramOptionConfig& instance() noexcept;

            /**
             * @brief Initializes the configuration from CLI args.
             * 
             * This must be called once in the main entry before using any accessors.
             * 
             * @param argc Argument count.
             * @param argv Argument vector.
             */
            void initialize(int argc, char const *argv[]);

            /**
             * @brief Returns the main CLI command (e.g., run, build).
             * 
             * @return Parsed command string.
             */
            const std::string& getCommand() const; 


            /**
             * @brief Returns the full parsed variables map.
             * 
             * @return Const reference to variable map.
             */
            const po::variables_map& getVarMap() const;

            /**
             * @brief Alternative accessor to retrieve options.
             * 
             * @return Const reference to variable map.
             */
            const po::variables_map& options() const;

            /**
             * @brief Gets the path to the YAML configuration file.
             * 
             * This is typically passed using `--config`.
             * 
             * @return Path to config file.
             */
            std::string configPath() const;

            /**
             * @brief Returns the entry file to run (.lynx file).
             * 
             * Retrieved using `--entry`.
             * 
             * @return Path to entry point file.
             */
            std::string entryFile() const;

            /**
             * @brief Indicates if debug mode is enabled.
             * 
             * Set using `--debug`.
             * 
             * @return true if debug mode is on.
             */
            bool isDebug() const;

            /**
             * @brief Indicates if dry-run mode is enabled.
             * 
             * Set using `--dry-run`.
             * 
             * @return true if dry-run is active.
             */
            bool isDryRun() const;

            /**
             * @brief Indicates if verbose logging is requested.
             * 
             * Set using `--verbose`.
             * 
             * @return true if verbose is enabled.
             */
            bool isVerbose() const;

            /**
             * @brief Checks if syntax highlighting is enabled.
             * 
             * @return true if syntax highlighting is turned on.
             */
            bool syntaxHighLight() const;

            /**
             * @brief Checks if source emission is required.
             * 
             * Relevant when generating intermediate or output code.
             * 
             * @return true if emitting source.
             */
            bool emitSource() const;

            /**
             * @brief Gets the custom output path (e.g., --output).
             * 
             * @return Output file or directory path.
             */
            std::string outputPath() const;

            /**
             * @brief Gets the requested state (e.g., visualization or compile phase).
             * 
             * Used for advanced flags like `--state`.
             * 
             * @return String representing the desired internal state.
             */
            std::string showState() const;

            /**
             * @brief Prints CLI banner, usage, and command help.
             * 
             * Called when user requests help or inputs an unknown command.
             */
            void printBanner() const;

            /**
             * @brief Load application root directory 
             * 
             * @param configFilePath 
             */
            void loadYamlConfig(const std::string& configFilePath);
    };

} 

#endif 
