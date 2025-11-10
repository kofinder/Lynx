#include "GCDashboard.hpp"

namespace LynxCore {

    void GCDashboard::start() {
        if (running.exchange(true)) return;

        monitorThread = std::thread([this]() {
            GCLogger::info("[GCDashboard] Starting live heap monitor...");
            while (running) {
                printSnapshot();
                std::this_thread::sleep_for(std::chrono::milliseconds(refreshIntervalMs));
            }
            GCLogger::warn("[GCDashboard] Stopped live monitor.");
        });
    }

    void GCDashboard::stop() {
        if (!running.exchange(false)) return;
        if (monitorThread.joinable()) monitorThread.join();
    }

    void GCDashboard::printSnapshot() {
        size_t edenCount = collector.getEdenCount();
        size_t survivorCount = 0;
        size_t oldCount = collector.getOldCount();

        int totalPromoted = collector.getTotalPromoted();
        int totalReclaimed = collector.getTotalReclaimed();
        int gcEvents = collector.getGCEvents();

        double avgAge = collector.getAverageAge();
        double oldUtil = collector.getOldUtilization();
        double youngUtil = collector.getYoungUtilization();

        std::ostringstream oss;
        oss << "\033[1;36m=== GC Dashboard Snapshot ===\033[0m\n"
            << "Total GC Events  : " << gcEvents << "\n"
            << "Total Promoted   : " << totalPromoted << "\n"
            << "Total Reclaimed  : " << totalReclaimed << "\n"
            << "Avg Object Age   : " << std::fixed << std::setprecision(2) << avgAge << "\n"
            << "Young Utilization: " << std::fixed << std::setprecision(1) << youngUtil << "%\n"
            << "Old Utilization  : " << std::fixed << std::setprecision(1) << oldUtil << "%\n";

        GCLogger::info(oss.str());
        GCLogger::visualizeHeap(edenCount, survivorCount, oldCount);
    }
}