/**
 * @file GCCollector.hpp
 * @brief Generational garbage collector for LynxGC (Eden + Old model).
 * 
 * The `GCCollector` class provides a lightweight, generational garbage collection
 * simulation integrated with the Boehm-Demers-Weiser GC. It tracks object allocations,
 * promotions, and reclamations while exposing metrics for visualization in tools like
 * `GCDashboard`.
 * 
 * Responsibilities:
 *  - Manage two generations: Eden (young) and Old (tenured)
 *  - Track allocation metadata, age, and promotion cycles
 *  - Simulate minor and major GC phases with detailed logging
 *  - Integrate safely with Boehm GC's automatic memory management
 * 
 * @note This is a *hybrid model* — Boehm GC handles actual reclamation, while LynxGC
 *       simulates generational behavior for research and instrumentation.
 * 
 * @see GCAllocator
 * @see GCFinalizer
 * @see GCDashboard
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

 #ifndef LYNX_CORE_GC_COLLECTOR_HPP
 #define LYNX_CORE_GC_COLLECTOR_HPP
 
 #include <gc.h>
 #include <vector>
 #include <mutex>
 #include <atomic>
 #include <chrono>
 #include <iomanip>
 #include <iostream>
 #include <numeric>
 #include "GCGeneration.hpp"
 #include "GCLogger.hpp"
 
 namespace LynxCore {
 
     /**
      * @class GCCollector
      * @brief Core generational garbage collector for LynxGC.
      * 
      * Manages allocation, promotion, and cleanup of managed objects.
      * Designed to work safely alongside Boehm GC while providing analytic
      * visibility into object lifetime and heap utilization.
      */
     class GCCollector {

        private:
    
            mutable std::mutex mutex_;
            std::vector<GCObject> eden_;          ///< Young generation objects.
            std::vector<GCObject> old_;           ///< Old generation objects.
            int promotionAge_;                    ///< Promotion age threshold.
            size_t edenThreshold_;                ///< Eden object count threshold.

            std::atomic<int> totalPromoted_;      ///< Number of promoted objects.
            std::atomic<int> totalReclaimed_;     ///< Number of reclaimed objects.
            std::atomic<int> gcEvents_;           ///< GC event counter.

        public:
        
            /**
             * @brief Constructs a GCCollector with configurable thresholds.
             * 
             * @param promotionAge  Number of cycles before an object is promoted.
             * @param edenThreshold Maximum object count before triggering Minor GC.
             */
            explicit GCCollector(int promotionAge = 2, size_t edenThreshold = 100)
                : promotionAge_(promotionAge), edenThreshold_(edenThreshold),
                totalPromoted_(0), totalReclaimed_(0), gcEvents_(0) {
                GC_INIT();
                GCLogger::info("[GCCollector] Boehm GC initialized.");
            }
    
            /** @brief Destructor — Boehm GC automatically reclaims memory. */
            ~GCCollector() {
                GCLogger::warn("[GCCollector] Shutting down collector; clearing tracked metadata.");
                std::lock_guard<std::mutex> lock(mutex_);
                eden_.clear();
                old_.clear();
            }

            void registerObject(void* obj, size_t size, Generation gen, const std::string& typeName) {
                std::lock_guard<std::mutex> lock(mutex_);
                eden_.push_back({obj, size, gen, 0, false, typeName});
            }        
            
            /**
             * @brief Allocates a new object in Eden space.
             * 
             * @tparam T Type of object.
             * @param args Constructor arguments.
             * @return Pointer to the constructed object.
             */
            template <typename T, typename... Args>
            [[nodiscard]] T* allocate(Args&&... args) {
                std::lock_guard<std::mutex> lock(mutex_);
    
                if (eden_.size() >= edenThreshold_) {
                    performMinorGC();
                }
    
                void* mem = GC_MALLOC(sizeof(T));
                if (!mem) throw std::bad_alloc();
    
                T* obj = new(mem) T(std::forward<Args>(args)...);
                eden_.push_back({obj, sizeof(T), Generation::YOUNG, 0, false});
                return obj;
            }

            /**
             * @brief Determines if an object is considered alive.
             * 
             * This mock implementation randomly marks objects based on their address.
             */
            [[nodiscard]] bool isAlive(const GCObject& obj) const noexcept;
    
            /**
             * @brief Simulates a Minor GC (Eden → Old promotion).
             */
            void performMinorGC();
    
            /**
             * @brief Simulates a Major GC (Old generation compaction & cleanup).
             */
            void performMajorGC();
    
            /**
             * @brief Logs heap usage metrics for the current GC phase.
             * 
             * @param phase Name of the GC phase (e.g., "Minor", "Major").
             * @param promoted Number of promoted objects.
             * @param reclaimed Number of reclaimed objects.
             * @param ms Duration in milliseconds.
             */
            void logHeapUsage(const std::string& phase, int promoted, int reclaimed, double ms) const;

        
            [[nodiscard]] size_t getEdenCount() const noexcept { return eden_.size(); }
            [[nodiscard]] size_t getOldCount() const noexcept { return old_.size(); }
            [[nodiscard]] int getTotalPromoted() const noexcept { return totalPromoted_.load(); }
            [[nodiscard]] int getTotalReclaimed() const noexcept { return totalReclaimed_.load(); }
            [[nodiscard]] int getGCEvents() const noexcept { return gcEvents_.load(); }
            [[nodiscard]] size_t getEdenThreshold() const noexcept {  return edenThreshold_; }
            
            [[nodiscard]] double getAverageAge() const noexcept;
            [[nodiscard]] double getOldUtilization() const noexcept;
            [[nodiscard]] double getYoungUtilization() const noexcept;
            [[nodiscard]] std::vector<std::string> getEdenTypes() const;

    };
 
 }
 
 #endif
 