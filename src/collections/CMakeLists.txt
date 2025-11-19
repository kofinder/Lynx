
set(LIB_MODULE_ADAPTER_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/ArrayAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/DictAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/GraphAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/ListAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/MapAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/QueueAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/SetAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/StackAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/TreeAdapter.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/adapter/VectorAdapter.hpp 
)

set(LIB_MODULE_FACTORY_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/factory/CollectionFactory.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/factory/SortingStrategyFactory.hpp 
)

set(LIB_MODULE_INTERFACES_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/IMap.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/IQueue.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/IStack.hpp  
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/ICollection.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SortingStrategy.hpp
)

set(LIB_MODULE_KINDS_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/kinds/CollectionKind.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/kinds/SortingStrategyKind.hpp 
)

set(LIB_MODULE_STRATEGY_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/BubbleSortStrategy.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/DefaultSortStrategy.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/InsertionSortStrategy.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/MergeSortStrategy.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/NoSortStrategy.hpp 
)

add_library(LynxCollection STATIC 
  ${LIB_MODULE_ADAPTER_SOURCES} 
  ${LIB_MODULE_FACTORY_SOURCES}
  ${LIB_MODULE_INTERFACES_SOURCES} 
  ${LIB_MODULE_KINDS_SOURCES}
  ${LIB_MODULE_STRATEGY_SOURCES}
)

target_link_libraries(LynxCollection PRIVATE LynxLogger)

target_include_directories(LynxCollection PUBLIC ${LIBRARY_INCLUDES} )



