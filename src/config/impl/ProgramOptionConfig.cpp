#include "ProgramOptionConfig.hpp"
#include "ProgramOptionConstant.hpp"
#include <logger/Logger.hpp>
#include <yaml-cpp/yaml.h>

using namespace LynxLogger;

namespace LynxProgramConfig {

    bool ProgramOptionConfig::is_initialized = false;

    ProgramOptionConfig& ProgramOptionConfig::instance() noexcept {
        static ProgramOptionConfig _instance;
        return _instance;
    }

    const std::string& ProgramOptionConfig::getCommand() const {
        return instance().cmd;
    }

    const po::variables_map& ProgramOptionConfig::getVarMap() const {
        return instance().vm;
    }

    void ProgramOptionConfig::initialize(int argc, char const* argv[]) {
        if (is_initialized) {
            std::cerr << "[Warning] Detected multiple initializations of ProgramOptionConfig!\n";
            return;
        }
    
        auto& self = ProgramOptionConfig::instance();
        is_initialized = true;
    
        if (argc < 2) {
            self.printBanner();
            std::exit(EXIT_FAILURE);
        }
    
        self.cmd = argv[1];
        std::vector<std::string> args(argv + 2, argv + argc);
    
        po::options_description general(general_opts_description.data());
        const auto help_option     = std::string(command_help_key) + "," + std::string(command_help_shortcut);
        const auto config_option   = std::string(option_config_key) + "," + std::string(option_config_shortcut);
        const auto verbose_option  = std::string(option_verbose_key) + "," + std::string(option_verbose_shortcut);
        
        general.add_options()
            (config_option.c_str(), po::value<std::string>()->default_value(config_file.data()), option_config_description.data())
            (verbose_option.c_str(), po::bool_switch()->default_value(false), option_verbose_description.data())
            (help_option.c_str(), command_help_description.data())
            ("version", "Show version info and exit");
    
        po::options_description run_opts(run_opts_description.data());
        const auto entry_option   = std::string(option_entry_key) + "," + std::string(option_entry_shortcut);
        const auto debug_option   = std::string(option_debug_key) + "," + std::string(option_debug_shortcut);
        const auto dry_option     = std::string(option_dryrun_key) + "," + std::string(option_dryrun_shortcut);
        
        run_opts.add_options()
            (entry_option.c_str(), po::value<std::string>(), option_entry_description.data())
            (debug_option.c_str(), po::bool_switch(), option_debug_description.data())
            (dry_option.c_str(), po::bool_switch(), option_dryrun_description.data());
    
        po::options_description misc_opts(misc_opts_description.data());
        const auto highlight_option = std::string(option_highlight_key) + "," + std::string(option_highlight_shortcut);
        
        misc_opts.add_options()
            (highlight_option.c_str(), po::bool_switch(), option_highlight_description.data())
            ("emit,l", po::bool_switch(), "Emit intermediate code")
            ("output,o", po::value<std::string>(), "Output path")
            ("state,s", po::value<std::string>(), "State visualization info");
    
        po::options_description create_opts("Create Options");
        create_opts.add_options()
            ("project", po::value<std::string>(), "Project name to create")
            ("type", po::value<std::string>()->default_value("generic"),
             "Type of project to create (web, console, etc.)"); 

        po::options_description generate_opts("Generate Options");
        generate_opts.add_options()
            ("subcommand", po::value<std::string>(), "Subcommand to generate (e.g., module, component, plugin)")
            ("name", po::value<std::string>(), "Name of the item to generate")
            ("project", po::value<std::string>()->default_value("."), "Project path for generation");

        po::options_description all(all_opts_description.data());
        all.add(general).add(run_opts).add(misc_opts);
        if (self.cmd == "create") all.add(create_opts);
        if (self.cmd == "generate") all.add(generate_opts);
    
        if (self.cmd == "help") {
            self.printBanner();
            std::exit(EXIT_SUCCESS);
        }
    
        try {

            po::store(po::command_line_parser(args).options(all).run(), self.vm);
            po::notify(self.vm);
    
            if (self.vm.count("help")) {
                std::cout << "Usage: lynx " << self.cmd << " [options]\n";
                std::cout << all << "\n";
                std::exit(EXIT_SUCCESS);
            }
    
            if (self.vm.count("version")) {
                std::cout << "Lynx CLI version 0.1.0\n";
                std::exit(EXIT_SUCCESS);
            }
    
            if (self.vm.count("config")) {
                const std::string yamlConfig = self.vm["config"].as<std::string>();
                self.loadYamlConfig(yamlConfig);
            }
    
        } catch (const po::error& e) {
            std::cerr << "Option parsing error: " << e.what() << '\n';
            std::cout << "Usage: lynx " << self.cmd << " [options]\n";
            std::cout << all << "\n";
            std::exit(EXIT_FAILURE);
        }
    }    

    void ProgramOptionConfig::loadYamlConfig(const std::string& configFilePath) {
        try {
            YAML::Node config = YAML::LoadFile(configFilePath);

            fs::path configPath(configFilePath); 
            fs::path rootDir = configPath.parent_path();

            
            std::string entry = config["entry"].as<std::string>();
            boost::filesystem::path rootFile = rootDir / entry;

            entrySource = rootFile.string();
            rootPath = rootDir.string();
            configFile = configFilePath;


            if (config["settings"] && config["settings"]["build_dir"]) {
                buildDir = rootPath + "/" + config["settings"]["build_dir"].as<std::string>();
            } else {
                buildDir = rootPath + "/" + "build";
            }

            for (const auto& folder : config["folders"]) {
                boost::filesystem::path folderPath = rootDir / folder.as<std::string>();
                if (boost::filesystem::exists(folderPath) && boost::filesystem::is_directory(folderPath)) {
                    sourceFolders.emplace_back(folderPath.string());
                } else {
                    std::cerr << "[Warning] Folder not found or not a directory: " << folderPath << "\n";
                }
            }
            
            bool debugMode = config["settings"]["debug"].as<bool>();
            std::cout << "Debug mode: " << (debugMode ? "ON" : "OFF") << "\n";
        } catch (const YAML::BadFile& e) {
            LOG_ERROR("Could not open config file '{}': {}", configFilePath, e.what());
        } catch (const YAML::ParserException& e) {
            LOG_ERROR("YAML parse error: {}", e.what());
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error loading YAML config: {}", e.what());
        }
    }
    
    const po::variables_map& ProgramOptionConfig::options() const {
        return instance().vm;
    }

    std::string ProgramOptionConfig::configPath() const {
        try {
            if (vm.contains("config")) {
                return vm["config"].as<std::string>();
            }
            return config_file.data();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--config' to string: {}", e.what());
            return "";
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--config': {}", e.what());
            return "";
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--config' flag.");
            return "";
        }
    }

    std::string ProgramOptionConfig::entryFile() const {
        try {
            if (vm.contains("entry")) {
                return vm["entry"].as<std::string>();
            }
            return entry_file.data();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--entry' to string: {}", e.what());
            return "";
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--entry': {}", e.what());
            return "";
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--entry' flag.");
            return "";
        }
    }
    
    bool ProgramOptionConfig::isDebug() const {
        try {
            return vm.count("debug") && vm["debug"].as<bool>();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--debug' to bool: {}", e.what());
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--debug': {}", e.what());
            return false;
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--debug' flag.");
            return false;
        } 
    }

    bool ProgramOptionConfig::isDryRun() const {
        try {
            return vm.count("dry-run") && vm["dry-run"].as<bool>();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--dry-run' to bool: {}", e.what());
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--dry-run': {}", e.what());
            return false;
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--dry-run' flag.");
            return false;
        } 
    }

    bool ProgramOptionConfig::isVerbose() const {
        try {
            return vm.count("verbose") && vm["verbose"].as<bool>();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--verbose' to bool: {}", e.what());
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--verbose': {}", e.what());
            return false;
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--verbose' flag.");
            return false;
        }   
    }

    bool ProgramOptionConfig::syntaxHighLight() const {
        try {
            return vm.count("highlight") && vm["highlight"].as<bool>();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--highlight' to bool: {}", e.what());
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--highlight': {}", e.what());
            return false;
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--highlight' flag.");
            return false;
        } 
    }

    
    bool ProgramOptionConfig::emitSource() const {
        try {
            return vm.count("emit") && vm["emit"].as<bool>();
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--emit' to bool: {}", e.what());
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--emit': {}", e.what());
            return false;
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--emit' flag.");
            return false;
        } 
    }

    std::string ProgramOptionConfig::outputPath() const {
        try {
            if (vm.contains("output")) {
                return vm["output"].as<std::string>();
            }
            return "";    
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--output' to string: {}", e.what());
            return "";
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--output': {}", e.what());
            return "";
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--output' flag.");
            return "";
        } 
    }
    
    std::string ProgramOptionConfig::showState() const {
        try {
            if (vm.contains("state")) {
                return vm["state"].as<std::string>();
            }
            return "";
        } catch (const boost::bad_any_cast& e) {
            LOG_ERROR("Failed to cast option '--output' to string: {}", e.what());
            return "";
        } catch (const std::exception& e) {
            LOG_ERROR("Unexpected error while accessing '--output': {}", e.what());
            return "";
        } catch (...) {
            LOG_ERROR("Unknown error occurred while checking '--output' flag.");
            return "";
        } 
    }

    void ProgramOptionConfig::printBanner() const {
        std::cout << "🦊 Lynx CLI - Modular Language Toolkit\n\n";

        std::cout << "📌 Usage:\n";
        std::cout << "lynx <command> [options]\n\n";
    
        std::cout << "Available Commands:\n";
        std::cout << " run         Run a .lynx source file\n";
        std::cout << " build       Compile the project\n";
        std::cout << " analyze     Perform static analysis\n";
        std::cout << " create      Create a new project scaffold\n";
        std::cout << " clean       Clean up build files\n";
        std::cout << " test        Run unit tests\n";
        std::cout << " upgrade     Upgrade tools/dependencies\n";
        std::cout << " downgrade   Downgrade tools/dependencies\n";
        std::cout << " help        Show this help message\n\n";
    
        std::cout << "ℹ️ Use `lynx <command> --help` to see available options for a specific command.\n";    
    }
}