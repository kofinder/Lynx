/**
 * @file GCGeneration.hpp
 * @brief Defines object metadata and generation model for LynxGC.
 * 
 * This header declares the `GCObject` structure and `Generation` enum,
 * representing the metadata tracked by the Lynx garbage collector.
 * Each managed object is associated with its generation, age, and
 * mark state. This data is purely *analytic* — Boehm GC performs
 * actual memory reclamation.
 * 
 * Responsibilities:
 *  - Define the object lifetime model (Eden → Old)
 *  - Track per-object metadata such as age and size
 *  - Support visualization and GC metric analysis
 * 
 * @see GCCollector
 * @see GCAllocator
 * @see GCDashboard
 * 
 * @note The GC does *not* manage raw memory manually — this is 
 *       compatible with the Boehm-Demers-Weiser GC runtime.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 8, 2025
 */

 #ifndef LYNX_CORE_GC_GENERATION_HPP
 #define LYNX_CORE_GC_GENERATION_HPP
 
 #include <cstddef>
 #include <string>
 
 namespace LynxCore {
 
 
     enum class Generation { YOUNG, OLD };
 
     /**
      * @struct GCObject
      * @brief Metadata describing a managed object.
      * 
      * Each tracked object in LynxGC carries this metadata for 
      * simulation and analytics purposes. The Boehm GC manages
      * actual memory allocation and reclamation.
    */
    struct GCObject {
        void* ptr;               ///< Pointer to the managed object.
        std::size_t size;        ///< Allocated size of the object.
        Generation gen;          ///< Current generation.
        int age;                 ///< Number of GC cycles survived.
        bool marked;             ///< Used for marking reachability.
        std::string typeName;    ///< Type name for logging/debugging

        GCObject() 
            : ptr(nullptr), size(0), gen(Generation::YOUNG), age(0), marked(false), typeName("unknown") {}

        GCObject(void* p, std::size_t s, Generation g, int a, bool m, const std::string& tName = "unknown") 
            : ptr(p), size(s), gen(g), age(a), marked(m), typeName(tName) {}

        [[nodiscard]] std::string generationName() const noexcept {
            return (gen == Generation::YOUNG) ? "YOUNG" : "OLD";
        }
    };
 } 
 
 #endif 
 