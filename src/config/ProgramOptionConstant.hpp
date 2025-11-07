/**
 * @file ProgramOptionConstant.hpp
 * @brief Defines constant values and metadata for CLI commands and options in the Lynx compiler.
 * 
 * This header centralizes all string literals, default paths, and descriptions used for command-line
 * interface (CLI) commands, options, and shortcuts. It ensures consistency across the Lynx compiler
 * tools and simplifies maintenance when adding or updating CLI functionality.
 * 
 * **Key Responsibilities:**
 * - Provide default filenames and entry points for Lynx projects (`.lynx` files, config files).
 * - Define string keys, shortcuts, and descriptions for core commands like `run`, `build`, `analyze`, etc.
 * - Define common CLI options including `--config`, `--entry`, `--debug`, `--dry-run`, `--verbose`, and `--highlight`.
 * - Support additional commands for versioning, project initialization, system checks, and package management.
 * 
 * **Used By:**
 * - ProgramOptionConfig and CLI parsers to validate, display, and execute commands.
 * - Build, run, and analysis subsystems to interpret user input consistently.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_PROGRAM_OPTION_CONSTANT_HPP
#define LYNX_PROGRAM_OPTION_CONSTANT_HPP

#include <string_view>
#include <array>

namespace LynxProgramConfig {

    inline constexpr std::string_view file_pefix = ".lynx";
    inline constexpr std::string_view entry_file = "toy/main.lynx";
    inline constexpr std::string_view config_file = "toy/app_config.yaml";

    // ======= Options =======
    inline constexpr std::string_view general_opts_description = "General Options";
    inline constexpr std::string_view run_opts_description = "Run Options";
    inline constexpr std::string_view misc_opts_description = "Misc Options";
    inline constexpr std::string_view all_opts_description = "Allowed options";

    // === Core Commands ===
    inline constexpr std::string_view command_help_key        = "help";
    inline constexpr std::string_view command_help_shortcut   = "h";
    inline constexpr std::string_view command_help_description = "Show help message";

    inline constexpr std::string_view command_run_key         = "run";
    inline constexpr std::string_view command_run_description = "Run the Lynx program";

    inline constexpr std::string_view command_build_key       = "build";
    inline constexpr std::string_view command_build_description = "Build the Lynx project";

    inline constexpr std::string_view command_analyze_key     = "analyze";
    inline constexpr std::string_view command_analyze_description = "Perform static analysis";

    inline constexpr std::string_view command_create_key      = "create";
    inline constexpr std::string_view command_create_description = "Create new Lynx project scaffold";

    inline constexpr std::string_view command_clean_key       = "clean";
    inline constexpr std::string_view command_clean_description = "Clean generated files and output";

    inline constexpr std::string_view command_test_key        = "test";
    inline constexpr std::string_view command_test_description = "Run unit and integration tests";

    inline constexpr std::string_view command_upgrade_key     = "upgrade";
    inline constexpr std::string_view command_upgrade_description = "Upgrade Lynx dependencies/tools";

    inline constexpr std::string_view command_downgrade_key   = "downgrade";
    inline constexpr std::string_view command_downgrade_description = "Downgrade Lynx dependencies/tools";

    // === Common CLI Options ===
    inline constexpr std::string_view option_config_key       = "config";
    inline constexpr std::string_view option_config_shortcut  = "c";
    inline constexpr std::string_view option_config_description = "YAML configuration file";

    inline constexpr std::string_view option_entry_key        = "entry";
    inline constexpr std::string_view option_entry_shortcut   = "e";
    inline constexpr std::string_view option_entry_description = "Main entry file (.lynx)";

    inline constexpr std::string_view option_debug_key        = "debug";
    inline constexpr std::string_view option_debug_shortcut   = "d";
    inline constexpr std::string_view option_debug_description = "Enable debug mode";

    inline constexpr std::string_view option_dryrun_key       = "dry-run";
    inline constexpr std::string_view option_dryrun_shortcut  = "n";
    inline constexpr std::string_view option_dryrun_description = "Simulate without execution";

    inline constexpr std::string_view option_verbose_key      = "verbose";
    inline constexpr std::string_view option_verbose_shortcut = "v";
    inline constexpr std::string_view option_verbose_description = "Enable verbose logging";

    inline constexpr std::string_view option_highlight_key      = "highlight";
    inline constexpr std::string_view option_highlight_shortcut = "hl";
    inline constexpr std::string_view option_highlight_description = "Enable syntax highlighting";


    inline constexpr std::string_view option_optimize_key     = "optimize";
    inline constexpr std::string_view option_optimize_shortcut = "O";
    inline constexpr std::string_view option_optimize_description = "Enable optimization flags";

    inline constexpr std::string_view command_version_key     = "version";
    inline constexpr std::string_view command_version_description = "Display current version";

    inline constexpr std::string_view command_doctor_key      = "doctor";
    inline constexpr std::string_view command_doctor_description = "Check system environment and dependencies";

    inline constexpr std::string_view command_info_key        = "info";
    inline constexpr std::string_view command_info_description = "Show project and toolchain info";

    inline constexpr std::string_view command_config_key      = "config";
    inline constexpr std::string_view command_config_description = "Edit or view user configuration";

    inline constexpr std::string_view command_init_key        = "init";
    inline constexpr std::string_view command_init_description = "Initialize a new Lynx project";

    inline constexpr std::string_view command_list_key        = "list";
    inline constexpr std::string_view command_list_description = "List available modules or extensions";

    inline constexpr std::string_view command_update_key      = "update";
    inline constexpr std::string_view command_update_description = "Update tools and packages (alias: upgrade)";

    inline constexpr std::string_view command_rollback_key    = "rollback";
    inline constexpr std::string_view command_rollback_description = "Rollback tools and packages (alias: downgrade)";    
}

#endif 
