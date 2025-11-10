/**
 * @file GCMutator.hpp
 * @brief Mutator interface for managed object allocation and reference updates in LynxGC.
 * 
 * The GCMutator class represents the *application thread* or *mutator* in 
 * the garbage-collected system. It performs object allocations, updates 
 * references, and interacts with the GCAllocator and GCCollector to simulate 
 * the mutator’s role in a managed heap.
 * 
 * Responsibilities:
 *  - Allocate new managed objects in the young (eden) generation.
 *  - Update object references to simulate live heap activity.
 *  - Trigger or cooperate with garbage collection phases.
 * 
 * This class is not a real concurrent mutator in Boehm GC (which automatically 
 * handles thread safety and allocation barriers), but a high-level abstraction 
 * for debugging and experimental generational management.
 * 
 * Usage Example:
 * @code
 *  GCMutator mutator;
 *  auto* obj1 = mutator.allocate<int>(42);
 *  auto* obj2 = mutator.allocate<std::string>("Lynx GC Rocks!");
 *  mutator.updateReference(obj1, obj2);
 * @endcode
 * 
 * @note The mutator typically interacts with GCCollector and GCAllocator 
 *       to simulate heap lifecycles in managed systems like JVM or CLR.
 * 
 * @warning This class is intended for GC simulation and instrumentation only.
 *          It does not provide a real write barrier or card marking.
 * 
 * @see GCAllocator
 * @see GCCollector
 * @see GCFinalizer
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

 #ifndef LYNX_CORE_GC_MUTATOR_HPP
 #define LYNX_CORE_GC_MUTATOR_HPP
 
 #include <gc.h>
 #include <iostream>
 #include <vector>
 #include <unordered_map>
 #include <memory>
 #include <typeinfo>
 #include <string>
 #include "GCAllocator.hpp"
 #include "GCFinalizer.hpp"
 #include "GCIntegration.hpp"

 namespace LynxCore {
 
     /**
      * @class GCMutator
      * @brief Represents the mutator (application thread) in a GC-managed environment.
      * 
      * The GCMutator class simulates heap object creation and reference 
      * mutation. It acts as the "user program" that allocates and manipulates 
      * objects while the collector tracks their reachability.
    */

     class GCMutator {

        private:

            std::vector<void*> roots;
    
            std::unordered_map<void*, std::vector<void*>> referenceGraph;
 
        public:
        
            GCMutator() {
                static bool initialized = false;
                if (!initialized) {
                    GC_INIT();
                    initialized = true;
                    std::cout << "[GCMutator] Boehm GC initialized." << std::endl;
                }
            }
    
            /**
             * @brief Allocate a new managed object through GCAllocator.
             * 
             * @tparam T Type of the object to allocate.
             * @tparam Args Variadic arguments for object construction.
             * @param args Constructor arguments forwarded to T.
             * @return Pointer to the allocated managed object.
             */
            template <typename T, typename... Args>
            [[nodiscard]] T* allocate(Args&&... args) {
                
                // Allocate memory using GCAllocator
                T* obj = GCAllocator::allocate<T>(std::forward<Args>(args)...);

                roots.push_back(obj);

                // Register with global GCCollector if initialized
                if (GCIntegration::isInitialized()) {
                    GCIntegration::getCollector().registerObject(
                        obj, sizeof(T), Generation::YOUNG, typeid(T).name()
                    );
                }

                std::cout << "[GCMutator] Allocated object of type: " << typeid(T).name() << std::endl;
                return obj;
            }
    
            /**
             * @brief Update an object reference to simulate pointer mutation.
             * 
             * @param from Source object.
             * @param to Target object.
             */
            void updateReference(void* from, void* to) {
                referenceGraph[from].push_back(to);
                std::cout << "[GCMutator] Updated reference from " << from << " to " << to << std::endl;
            }
    
            /**
             * @brief Register a custom finalizer for an object.
             * 
             * @tparam T Type of object to finalize.
             * @param obj Pointer to the object.
             * @param finalizer Finalizer callback.
             */
            template <typename T>
            void registerFinalizer(T* obj, void (*finalizer)(void*, void*)) {
                GCFinalizer::registerFinalizer(obj, finalizer);
            }
    
            /**
             * @brief Print all known references managed by this mutator.
             */
            void printReferences() const {
                std::cout << "\n[GCMutator] Reference Graph:" << std::endl;
                for (const auto& [from, targets] : referenceGraph) {
                    std::cout << "  " << from << " -> ";
                    for (auto* to : targets)
                        std::cout << to << " ";
                    std::cout << std::endl;
                }
            }
    
            /**
             * @brief Return the list of GC root pointers.
             * @return const std::vector<void*>& Vector of roots.
             */
            [[nodiscard]] const std::vector<void*>& getRoots() const noexcept {
                return roots;
            }
    
            /**
             * @brief Force a manual garbage collection cycle.
             * 
             * Useful for testing, simulation, or instrumentation.
             */
            void triggerGC() const {
                std::cout << "[GCMutator] Triggering GC manually..." << std::endl;
                GC_gcollect();
            }
    };
 
 }
 
 #endif
 