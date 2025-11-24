/**
 * @file GCCollector.cpp
 * @brief Implementation of the GCCollector (Eden + Old model) for LynxGC.
 *
 * Implements minor/major GC simulation, logging, and metrics. Uses Boehm GC
 * for actual allocation/reclamation while providing a high-level view for
 * instrumentation and dashboarding.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

 #include <algorithm>
 #include <numeric>
 #include <chrono>
 #include "GCCollector.hpp" 

 namespace LynxCore {
     
    /// Perform a minor GC: scan Eden, promote survivors to Old (based on age), count reclaimed.
    void GCCollector::performMinorGC() {
        auto t0 = GCLogger::startTimer();
        GCLogger::info("[GCCollector] Minor GC started");
    
        std::lock_guard<std::mutex> lock(mutex_);
    
        int promoted = 0;
        int reclaimed = 0;
        std::vector<GCObject> survivors;
        survivors.reserve(eden_.size());
    
        for (auto &meta : eden_) {
            if (isAlive(meta)) {
                // Survivor: increase age and decide promotion
                GCObject copy = meta;
                copy.age++;
                if (copy.age >= promotionAge_) {
                    copy.gen = Generation::OLD;
                    old_.push_back(copy);
                    ++promoted;
                } else {
                    survivors.push_back(std::move(copy));
                }
            } else {
                // Dead: reclaimed (actual memory freed by Boehm GC)
                ++reclaimed;
            }
        }
    
        // Replace eden with survivors
        eden_.swap(survivors);
    
        totalPromoted_ += promoted;
        totalReclaimed_ += reclaimed;
        ++gcEvents_;
    
        double ms = GCLogger::stopTimer(t0);
        logHeapUsage("Minor GC", promoted, reclaimed, ms);
    }
    
    /// Perform a major GC: scan Old generation and remove dead objects.
    void GCCollector::performMajorGC() {
        auto t0 = GCLogger::startTimer();
        GCLogger::warn("[GCCollector] Major GC started");
    
        std::lock_guard<std::mutex> lock(mutex_);
    
        int reclaimed = 0;
        std::vector<GCObject> survivors;
        survivors.reserve(old_.size());
    
        for (auto &meta : old_) {
            if (isAlive(meta)) {
                // keep it, reset mark (metadata only)
                GCObject copy = meta;
                copy.marked = false;
                survivors.push_back(std::move(copy));
            } else {
                ++reclaimed;
            }
        }
    
        old_.swap(survivors);
        totalReclaimed_ += reclaimed;
        ++gcEvents_;
    
        double ms = GCLogger::stopTimer(t0);
        logHeapUsage("Major GC", 0, reclaimed, ms);
    }
    
    /// Centralized logging of heap usage; prints metrics and visualizes heap.
    void GCCollector::logHeapUsage(const std::string &phase, int promoted, int reclaimed, double ms) const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "[GC] Phase: " << phase
            << " | Eden: " << eden_.size()
            << " | Old: " << old_.size()
            << " | Promoted: " << promoted
            << " | Reclaimed: " << reclaimed
            << " | Time: " << ms << " ms";
        GCLogger::info(oss.str());
    
        // Visualize using logger helper (thread-safe)
        GCLogger::visualizeHeap(eden_.size(), /*survivor*/ 0, old_.size());
    }
    
    bool GCCollector::isAlive(const GCObject &obj) const noexcept {
        if (!obj.ptr) return false;
        return GC_base(obj.ptr) != nullptr;
    }
    
    double GCCollector::getAverageAge() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t totalCount = eden_.size() + old_.size();
        if (totalCount == 0) return 0.0;
    
        double sum = 0.0;
        for (const auto &o : eden_) sum += o.age;
        for (const auto &o : old_)  sum += o.age;
        return sum / static_cast<double>(totalCount);
    }
    
    double GCCollector::getYoungUtilization() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t total = eden_.size() + old_.size();
        if (total == 0) return 0.0;
        return static_cast<double>(eden_.size()) / static_cast<double>(total) * 100.0;
    }
    
    double GCCollector::getOldUtilization() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t total = eden_.size() + old_.size();
        if (total == 0) return 0.0;
        return static_cast<double>(old_.size()) / static_cast<double>(total) * 100.0;
    }

    std::vector<std::string> GCCollector::getEdenTypes() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::string> types;
        for (auto& obj : eden_) types.push_back(obj.typeName);
        return types;
    }
}
 