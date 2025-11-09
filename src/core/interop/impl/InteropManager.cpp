#include <mutex>
#include <gc.h>
#include <llvm/IR/Value.h>
#include "InteropManager.hpp"
#include "memory/MemoryManager.hpp"
#include <stdapi/src/ctor_runtime.h>

namespace LynxCore {

    static std::mutex interopMutex;

    void InteropManager::initialize() {
        registerFunction("GC_malloc", reinterpret_cast<void*>(&GC_malloc));
        registerFunction("GC_realloc", reinterpret_cast<void*>(&GC_realloc));
        registerFunction("GC_free", reinterpret_cast<void*>(&GC_free));    
        registerFunction("int_ctor", reinterpret_cast<void*>(&int_ctor));
        registerFunction("DateTime_ctor", reinterpret_cast<void*>(&datetime_ctor));
        registerFunction("File_ctor", reinterpret_cast<void*>(&file_ctor));
        registerFunction("pthread_create", reinterpret_cast<void*>(&pthread_create));
        registerFunction("pthread_join", reinterpret_cast<void*>(&pthread_join));
        registerFunction("pthread_self", reinterpret_cast<void*>(&pthread_self));
        registerFunction("pthread_equal", reinterpret_cast<void*>(&pthread_equal));
    }

    void* InteropManager::getFunction(const std::string& name) {
        auto it = getFunctionMap().find(name);
        return it != getFunctionMap().end() ? it->second : nullptr;
    }

    const std::unordered_map<std::string, void*>& InteropManager::getAll() {
        return getFunctionMap();
    }
    
    std::unordered_map<std::string, void*>& InteropManager::getFunctionMap() {
        static std::unordered_map<std::string, void*> functionMap;
        return functionMap;
    }
    
    std::mutex& InteropManager::getMutex() {
        static std::mutex mtx;
        return mtx;
    }
    
    void InteropManager::registerFunction(const std::string& name, void* ptr) {
        std::lock_guard lock(getMutex());
        getFunctionMap()[name] = ptr;
    }

    void InteropManager::shutdown() {
        std::lock_guard<std::mutex> lock(interopMutex);
        getFunctionMap().clear();
    }

}
