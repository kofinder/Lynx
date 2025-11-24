/**
 * @file GCAllocator.hpp
 * @brief Memory allocation interface for the Lynx Garbage Collector (LynxGC).
 * 
 * The GCAllocator class provides safe and efficient memory allocation utilities 
 * that integrate directly with the Boehm–Demers–Weiser Garbage Collector. 
 * It simplifies heap object creation by automatically managing memory through 
 * Boehm GC and supports both standard and atomic object allocations.
 * 
 * Responsibilities:
 *  - Provide type-safe, RAII-compatible object allocations.
 *  - Integrate with Boehm GC for automatic garbage collection.
 *  - Support atomic allocations for non-pointer-containing objects.
 * 
 * Usage Example:
 * @code
 *  auto* obj = GCAllocator::allocate<MyObject>(42, "example");
 *  auto* raw = GCAllocator::allocateAtomic<int>(123);
 * @endcode
 * 
 * @note Atomic allocations (`GC_MALLOC_ATOMIC`) should be used for objects that 
 *       do not contain pointers to other GC-tracked memory, to reduce scanning overhead.
 * @warning Memory returned from GCAllocator must not be manually freed with `delete`.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

 #ifndef LYNX_CORE_GC_ALLOCATOR_HPP
 #define LYNX_CORE_GC_ALLOCATOR_HPP
 
 #include <gc.h>
 #include <stdexcept>
 #include <utility>
 #include "GCGeneration.hpp"
 
 namespace LynxCore {
 
     /**
      * @class GCAllocator
      * @brief Provides allocation utilities integrated with Boehm GC.
      * 
      * The GCAllocator class offers templated allocation methods to 
      * construct managed objects using placement new within GC-allocated 
      * memory. The allocated objects are automatically traced and 
      * collected by Boehm GC, ensuring safe memory management without 
      * explicit deallocation.
    */
    class GCAllocator {

        public:
    
            /**
             * @brief Allocates and constructs a garbage-collected object.
             * 
             * Allocates memory using `GC_MALLOC`, then constructs an object 
             * of type `T` using placement new. The resulting object will be 
             * automatically managed by Boehm GC.
             * 
             * @tparam T Type of object to allocate.
             * @tparam Args Constructor argument types.
             * @param args Arguments forwarded to the constructor of `T`.
             * @return Pointer to the constructed, GC-managed object.
             * @throws std::bad_alloc If memory allocation fails.
             */
            template <typename T, typename... Args>
            [[nodiscard]] static T* allocate(Args&&... args) {
                void* mem = GC_MALLOC(sizeof(T));
                if (!mem) throw std::bad_alloc();
                return new (mem) T(std::forward<Args>(args)...);
            }
    
            /**
             * @brief Allocates and constructs a garbage-collected atomic object.
             * 
             * Uses `GC_MALLOC_ATOMIC` for allocations that do not contain 
             * pointers, reducing tracing overhead. This is ideal for simple 
             * data structures or immutable objects.
             * 
             * @tparam T Type of object to allocate.
             * @tparam Args Constructor argument types.
             * @param args Arguments forwarded to the constructor of `T`.
             * @return Pointer to the constructed, GC-managed object.
             * @throws std::bad_alloc If memory allocation fails.
             */
            template <typename T, typename... Args>
            [[nodiscard]] static T* allocateAtomic(Args&&... args) {
                void* mem = GC_MALLOC_ATOMIC(sizeof(T));
                if (!mem) throw std::bad_alloc();
                return new (mem) T(std::forward<Args>(args)...);
            }
    };
 }
 
 #endif
 