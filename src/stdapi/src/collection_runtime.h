/**
 * @file collection_runtime.h
 * @brief C runtime interface for manipulating various collection types.
 *
 * This API provides a simplified, opaque C interface to work with
 * collection data structures such as arrays, lists, sets, maps, stacks, etc.,
 * from the C side while leveraging underlying C++ STL or custom implementations.
 */

 #ifndef COLLECTION_RUNTIME_H
 #define COLLECTION_RUNTIME_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * @brief Opaque pointer representing a collection object.
  *
  * Internally backed by a C++ collection adapter.
  */
 typedef void* collection_t;
 
 /**
  * @brief Create a new collection instance of the given kind.
  *
  * @param kind Type of the collection from CollectionKind enum.
  * @return A new collection handle, or NULL on failure.
  */
 collection_t collection_create(int kind);
 
 /**
  * @brief Add an integer value to the collection.
  *
  * @param col Collection handle.
  * @param value Integer to add.
  */
 void collection_add_int(collection_t col, int value);
 
 /**
  * @brief Remove an integer value from the collection.
  *
  * @param col Collection handle.
  * @param value Integer to remove.
  */
 void collection_remove_int(collection_t col, int value);
 
 /**
  * @brief Check if the integer value exists in the collection.
  *
  * @param col Collection handle.
  * @param value Integer to check.
  * @return 1 if found, 0 otherwise.
  */
 int collection_contains_int(collection_t col, int value);
 
 /**
  * @brief Get the number of elements in the collection.
  *
  * @param col Collection handle.
  * @return Number of elements.
  */
 int collection_size(collection_t col);
 
 /**
  * @brief Clear all elements in the collection.
  *
  * @param col Collection handle.
  */
 void collection_clear(collection_t col);
 
 /**
  * @brief Destroy and free memory of the collection.
  *
  * @param col Collection handle.
  */
 void collection_destroy(collection_t col);
 
 /**
  * @brief Put a key-value pair into a map/dictionary.
  *
  * @param col Collection handle (must be COLLECTION_MAP or COLLECTION_DICT).
  * @param key String key.
  * @param value Integer value to associate.
  */
 void collection_put_str_int(collection_t col, const char* key, int value);
 
 /**
  * @brief Get the value associated with a string key in a map/dict.
  *
  * @param col Collection handle.
  * @param key String key.
  * @return Integer value associated, or 0 if not found.
  */
 int collection_get_str_int(collection_t col, const char* key);
 
 /**
  * @brief Check if the given string key exists in the map/dict.
  *
  * @param col Collection handle.
  * @param key String key.
  * @return 1 if exists, 0 otherwise.
  */
 int collection_has_key_str(collection_t col, const char* key);
 
 /**
  * @brief Remove a string key from the map/dict.
  *
  * @param col Collection handle.
  * @param key String key to remove.
  */
 void collection_remove_key_str(collection_t col, const char* key);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif // COLLECTION_RUNTIME_H
 