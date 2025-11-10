/**
 * @file GCFinalizer.hpp
 * @brief Finalization utilities for the Lynx Garbage Collector (LynxGC).
 * 
 * The GCFinalizer class provides an interface for registering custom 
 * destructors (finalizers) with the Boehm–Demers–Weiser Garbage Collector. 
 * Finalizers allow developers to perform cleanup or resource release 
 * operations automatically when objects are collected.
 * 
 * Responsibilities:
 *  - Register custom finalizers for GC-managed objects.
 *  - Provide a default finalizer for destructing C++ objects safely.
 *  - Integrate seamlessly with the Boehm GC lifecycle.
 * 
 * Usage Example:
 * @code
 *  struct Resource {
 *      ~Resource() { std::cout << "Releasing resource\n"; }
 *  };
 * 
 *  auto* res = GCAllocator::allocate<Resource>();
 *  GCFinalizer::registerFinalizer(res, &GCFinalizer::defaultFinalizer<Resource>);
 * @endcode
 * 
 * @note The Boehm GC may delay finalizer execution, and finalizers are 
 *       typically called in an arbitrary thread. Avoid using them for 
 *       thread-unsafe cleanup.
 * @warning Do not perform new allocations or complex logic inside a finalizer.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

 #ifndef LYNX_CORE_GC_FINALIZE_HPP
 #define LYNX_CORE_GC_FINALIZE_HPP
 
 #include <gc.h>
 #include <iostream>
 #include <stdexcept>
 #include <utility>
 
 namespace LynxCore {
 
    /**
      * @class GCFinalizer
      * @brief Provides registration and management of object finalizers.
      * 
      * The GCFinalizer class allows registration of custom destructors that 
      * are invoked automatically when GC-managed objects are reclaimed. It 
      * provides both user-defined and default finalizer utilities for 
      * resource cleanup in a managed runtime environment.
    */
     class GCFinalizer {
        
        public:
 
            /**
             * @brief Registers a custom finalizer function for a GC-managed object.
             * 
             * This function associates a user-provided callback that executes 
             * when the garbage collector determines the object is no longer reachable.
             * 
             * @tparam T Type of object being finalized.
             * @param obj Pointer to the GC-managed object.
             * @param finalizerFunc Function pointer to the finalizer.
             * @return void
             * @note The finalizer runs asynchronously when the object is collected.
             */
            template <typename T>
            static void registerFinalizer(T* obj, void (*finalizerFunc)(void*, void*)) {
                GC_register_finalizer(obj, finalizerFunc, nullptr, nullptr, nullptr);
            }
    
            /**
             * @brief Default finalizer for safely destructing objects.
             * 
             * Invokes the C++ destructor of the specified type `T` to ensure 
             * resource cleanup (RAII destruction). This is useful for classes 
             * with custom destructors that manage external resources.
             * 
             * @tparam T Type of object being finalized.
             * @param obj Pointer to the raw object memory.
             * @param client_data Reserved for user data (unused).
             * @return void
             */
            template <typename T>
            static void defaultFinalizer(void* obj, void* /*client_data*/) {
                T* o = static_cast<T*>(obj);
                if (o) {
                    o->~T();
                    std::cout << "[GCFinalizer] Destructor called for object of type " << typeid(T).name() << std::endl;
                }
            }
     };
 
 }
 
 #endif
 