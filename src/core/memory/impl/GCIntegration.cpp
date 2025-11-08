/**
 * @file GCIntegration.cpp
 * @brief Integration layer between Lynx runtime and Boehm GC system.
 * 
 * This file provides a unified interface for initializing, managing,
 * and shutting down the Lynx Garbage Collector subsystem. It wraps 
 * around Boehm GC and provides lifecycle hooks for the compiler and 
 * runtime environment. 
 * 
 * Responsibilities:
 *  - Initialize and finalize Boehm GC.
 *  - Manage background collector and dashboard threads.
 *  - Expose allocation and collection APIs for runtime integration.
 *  - Provide hooks for compiler-generated object management code.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 8, 2025
 */

 #include "GCIntegration.hpp"
 #include "GCCollector.hpp"
 #include "GCAllocator.hpp"
 #include "GCDashboard.hpp"
 #include "GCLogger.hpp"
 #include "GCFinalizer.hpp"
 
 #include <gc.h>
 #include <mutex>
 #include <memory>
 
 namespace LynxCore {
 
    namespace {
        std::unique_ptr<GCCollector> g_collector;
        std::unique_ptr<GCDashboard> g_dashboard;
        std::mutex g_mutex;
        bool g_initialized = false;
    }

    void GCIntegration::initialize(bool enableDashboard) {
        std::lock_guard<std::mutex> lock(g_mutex);

        if (g_initialized) {
            GCLogger::warn("[GCIntegration] GC already initialized, skipping re-init.");
            return;
        }

        GC_INIT();
        GCLogger::info("[GCIntegration] Boehm GC initialized.");

        g_collector = std::make_unique<GCCollector>();
        GCLogger::info("[GCIntegration] Collector created.");

        if (enableDashboard) {
            g_dashboard = std::make_unique<GCDashboard>(*g_collector);
            g_dashboard->start();
            GCLogger::info("[GCIntegration] Live dashboard started.");
        }

        g_initialized = true;
    }

    void GCIntegration::shutdown() {
        std::lock_guard<std::mutex> lock(g_mutex);

        if (!g_initialized) {
            GCLogger::warn("[GCIntegration] GC not initialized, nothing to shut down.");
            return;
        }

        if (g_dashboard) {
            g_dashboard->stop();
            g_dashboard.reset();
        }

        g_collector.reset();
        g_initialized = false;

        GCLogger::warn("[GCIntegration] GC subsystem shut down successfully.");
    }

    void GCIntegration::triggerMinorGC() {
        if (!g_collector) return;
        g_collector->performMinorGC();
    }

    void GCIntegration::triggerMajorGC() {
        if (!g_collector) return;
        g_collector->performMajorGC();
    }

    GCCollector& GCIntegration::getCollector() {
        if (!g_collector) {
            throw std::runtime_error("[GCIntegration] Collector not initialized!");
        }
        return *g_collector;
    }

    bool GCIntegration::isInitialized() noexcept {
        return g_initialized;
    }
 
 } 
 