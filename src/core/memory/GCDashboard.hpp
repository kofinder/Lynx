/**
 * @file GCDashboard.hpp
 * @brief Live garbage collection monitoring and visualization tool for LynxGC.
 * 
 * The `GCDashboard` class provides a real-time dashboard to monitor the 
 * performance and statistics of the garbage collector (GC). It continuously
 * displays live heap utilization, GC event counts, promotions, and reclamations.
 * 
 * Responsibilities:
 *  - Periodically poll the GCCollector for heap and collection metrics.
 *  - Print color-coded live snapshots to the console.
 *  - Provide thread-safe monitoring that runs concurrently with the collector.
 * 
 * The dashboard helps developers visualize the state of the generational 
 * heap, including Eden, Survivor, and Old generations, in a dynamic and 
 * human-readable form.
 * 
 * Example:
 * @code
 *  GCCollector collector;
 *  GCDashboard dashboard(collector, 1000); // update every 1 second
 *  dashboard.start();
 *  ...
 *  dashboard.stop();
 * @endcode
 * 
 * @note The dashboard is purely diagnostic and has no effect on GC behavior.
 * 
 * @see GCCollector
 * @see GCLogger
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

 #ifndef LYNX_CORE_GC_DASHBOARD_HPP
 #define LYNX_CORE_GC_DASHBOARD_HPP
 
 #include "GCCollector.hpp"
 #include "GCLogger.hpp"
 #include <atomic>
 #include <thread>
 #include <chrono>
 #include <iomanip>
 #include <sstream>
 
 namespace LynxCore {
 
    /**
      * @class GCDashboard
      * @brief Periodic, live monitoring dashboard for GC metrics and heap activity.
      * 
      * The `GCDashboard` runs in a separate thread, periodically querying 
      * `GCCollector` for current statistics. It then formats and prints a 
      * live summary including total promotions, reclamations, utilization rates, 
      * and average object age.
    */
     class GCDashboard {

        private:

            GCCollector& collector;          ///< Reference to active GCCollector instance.
            int refreshIntervalMs;           ///< Refresh interval in milliseconds.
            std::atomic<bool> running;       ///< Thread running state flag.
            std::thread monitorThread;       ///< Background monitoring thread.

        public:
        
            /**
             * @brief Construct a new GCDashboard instance.
             * 
             * @param collector Reference to a `GCCollector` instance.
             * @param refreshMs Dashboard refresh interval in milliseconds (default: 2000ms).
             */
            explicit GCDashboard(
                GCCollector& keeper, 
                int refreshMs = 2000
            ) : collector(keeper), refreshIntervalMs(refreshMs), running(false) {}
    
            /**
             * @brief Destructor. Ensures the monitoring thread is stopped safely.
             */
            ~GCDashboard() {
                stop();
            }
    
            /**
             * @brief Start the live dashboard thread.
             * 
             * If already running, this call has no effect.
             */
            void start();
    
            /**
             * @brief Stop the live monitoring thread and join it safely.
             */
            void stop();

            /**
             * @brief Print a formatted snapshot of GC and heap statistics.
             * 
             * The snapshot includes event counts, object age averages, and 
             * generation utilization percentages.
            */
            void printSnapshot();
    };
 
 }
 
 #endif
 