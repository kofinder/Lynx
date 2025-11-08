/**
 * @file GCIntegration.hpp
 * @brief Public API for Lynx Garbage Collector integration layer.
 * 
 * This header defines the static interface used by the Lynx compiler 
 * runtime to control and communicate with the Boehm-based GC subsystem.
 * It acts as a unified access point for allocation, collection, and 
 * diagnostic functions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 8, 2025
*/

#ifndef LYNX_CORE_GC_INTEGRATION_HPP
#define LYNX_CORE_GC_INTEGRATION_HPP

#include <atomic>
#include <memory>
#include "GCCollector.hpp"

namespace LynxCore {

    class GCIntegration {

        public:
        
            /**
             * @brief Initializes the Boehm GC and Lynx GC subsystems.
             * 
             * @param enableDashboard Whether to enable the live monitoring dashboard.
             */
            static void initialize(bool enableDashboard = true);

            /**
             * @brief Safely shuts down GC subsystems and background threads.
             */
            static void shutdown();

            /**
             * @brief Triggers a young generation (minor) GC cycle.
             */
            static void triggerMinorGC();

            /**
             * @brief Triggers a full old generation (major) GC cycle.
             */
            static void triggerMajorGC();

            /**
             * @brief Provides reference to the active GC collector.
             * 
             * @return Reference to the global GCCollector.
             */
            static GCCollector& getCollector();

            /**
             * @brief Checks whether the GC subsystem has been initialized.
             * 
             * @return true if initialized, false otherwise.
             */
            static bool isInitialized() noexcept;
    };

}

#endif 
