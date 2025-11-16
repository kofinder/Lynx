#include "SystemModuleLoader.hpp"
#include "SystemModuleRegistry.hpp"
#include "modules/IOModule.hpp"
#include <logger/Logger.hpp>

namespace LynxSystem {

    using namespace LynxLogger;

    bool SystemModuleLoader::initialized = false;

    void SystemModuleLoader::initializeCoreModules() {

        if (initialized) {
            LOG_WARN("SystemModuleLoader: Already initialized");
            return;
        }
        
        SystemModuleRegistry::registerModule("io", []() {
            return std::make_unique<IOModule>();
        });
    
        initialized = true;
    }
    
    void SystemModuleLoader::loadCustomModule(const std::string& name, std::function<std::unique_ptr<ISystemModule>()> factory) {
        SystemModuleRegistry::registerModule(name, std::move(factory));
    }
    
    void SystemModuleLoader::validateModules(const std::vector<std::string>& requiredModules) {
        for (const auto& name : requiredModules) {
            auto module = SystemModuleRegistry::getModule(name);
            if (!module) {
                LOG_ERROR("Required module '{}' not registered!", name);
            }
        }
    }
    
    void SystemModuleLoader::shutdown() {
        if (!initialized) {
            LOG_WARN("SystemModuleLoader::shutdown() called before initialization.");
            return;
        }
        
        try {
            SystemModuleRegistry::clear();
            initialized = false;
        } catch (const std::exception& ex) {
            LOG_ERROR("Error during SystemModuleLoader shutdown: {}", ex.what());
        } catch (...) {
            LOG_ERROR("Unknown error during SystemModuleLoader shutdown.");
        }    
    }

}