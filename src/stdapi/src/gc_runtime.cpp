/**
 * @file gc_runtime.cpp
 * @brief GC allocation and runtime entry points for Lynx stdapi module.
 *
 * This module bridges LLVM IR calls like:
 *     %p = call i8* @LYNX_GC_ALLOC_<classname>(i64 <size>)
 * to the internal Lynx GC system (MemoryManager, Boehm GC, and Dashboard).
 */

 #include "gc_runtime.h"
 #include <gc.h>
 #include <cstring>
 #include <iostream>
 #include <exception>

 #include <core/memory/GCGeneration.hpp>
 #include <core/memory/MemoryManager.hpp>
 #include <core/memory/GCAllocator.hpp>
 #include <core/memory/GCLogger.hpp>
 #include <core/memory/GCIntegration.hpp>
 #include <core/bindings/RuntimeBindingManager.hpp>
 
 using namespace LynxCore;
 
 // ---------------------------------------------------------------------------
 // Internal helper
 // ---------------------------------------------------------------------------
 
 static void register_allocation(void* ptr, std::size_t size, const char* typeName = "Unknown") noexcept {
     try {
         auto* mgr = RuntimeBindingManager::getBoundMemoryManager();
         if (mgr) {
            mgr->getCollector().registerObject(ptr, size, Generation::YOUNG, typeName);
         } else {
            GCLogger::warn("[stdapi.gc_runtime] MemoryManager not bound; skipping GC registration.");
         }

         if (GCIntegration::isInitialized()) {
            auto& collector = GCIntegration::getCollector();
            if (collector.getEdenCount() >= collector.getEdenThreshold()) {
                GCIntegration::triggerMinorGC();
            }
        }

     } catch (const std::exception& e) {
         GCLogger::warn(std::string("[stdapi.gc_runtime] Exception during registration: ") + e.what());
     } catch (...) {
         GCLogger::warn("[stdapi.gc_runtime] Unknown error during GC registration.");
     }
 }
 
 // ---------------------------------------------------------------------------
 // Public API exposed to LLVM IR
 // ---------------------------------------------------------------------------
 
 extern "C" {
 
 /**
  * @brief Generic GC allocator callable from LLVM IR or JIT code.
  *
  * @param size Allocation size in bytes.
  * @return Pointer to managed memory, or nullptr if allocation fails.
  */
 void* LYNX_GC_ALLOC_generic(uint64_t size) noexcept {
     try {
         if (size == 0) return nullptr;
 
        // Allocate from Boehm GC heap
         void* ptr = GC_malloc(static_cast<size_t>(size));
         if (!ptr) {
             std::cerr << "[LYNX_GC_ALLOC_generic] Allocation failed for size=" << size << std::endl;
             return nullptr;
         }
 
         std::memset(ptr, 0, static_cast<size_t>(size));
         register_allocation(ptr, static_cast<size_t>(size));
         return ptr;
 
     } catch (const std::exception& e) {
         std::cerr << "[LYNX_GC_ALLOC_generic] Exception: " << e.what() << std::endl;
         return nullptr;
     } catch (...) {
         std::cerr << "[LYNX_GC_ALLOC_generic] Unknown exception during allocation." << std::endl;
         return nullptr;
     }
 }
 
 /**
  * @brief Typed GC allocator (optional).
  *
  * Useful if you want dashboard to know which class allocated the object.
  */
 void* LYNX_GC_ALLOC_typed(uint64_t size, const char* typeName) noexcept {
    void* ptr = LYNX_GC_ALLOC_generic(size);
    if (ptr && typeName) {
        auto* mgr = RuntimeBindingManager::getBoundMemoryManager();
        if (mgr) {
            mgr->getCollector().registerObject(ptr, static_cast<size_t>(size), Generation::YOUNG, typeName);
        }
    }
    return ptr;
 }
 
 } // extern "C"
 