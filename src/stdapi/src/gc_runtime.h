#ifndef GC_RUNTIME_H
#define GC_RUNTIME_H

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief Registers a finalizer for an object allocated on the GC heap.
     * 
     * The finalizer is called when the GC collects the object.
     * 
     * @param obj Pointer to the GC-allocated object.
     * @param finalizer Function pointer: void finalizer(void* obj, void* client_data)
     * @param client_data Optional user data to pass to the finalizer (can be NULL)
     */
    static inline void register_finalizer(void* obj, void (*finalizer)(void*, void*), void* client_data);

    /**
     * @brief Example finalizer for File objects
     */
    void file_finalizer(void* obj, void* client_data);

#ifdef __cplusplus
}
#endif

#endif // COLLECTION_RUNTIME_H
