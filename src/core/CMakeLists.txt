# find_package(Threads REQUIRED)
# find_library(GC_LIB NAMES gc REQUIRED)

find_package(Threads REQUIRED)
find_library(GC_LIBRARY gc)
if(NOT GC_LIBRARY)
    message(FATAL_ERROR "Boehm GC library not found. Please install libgc.")
endif()

message(STATUS "Boehm GC OUTPUTS: ${GC_LIBRARY}")
message(STATUS "GC_LIB: ${GC_LIB}")
message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")

# Include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings
  ${CMAKE_CURRENT_SOURCE_DIR}/interop
  ${CMAKE_CURRENT_SOURCE_DIR}/memory
  ${CMAKE_CURRENT_SOURCE_DIR}/scheduler
  ${CMAKE_CURRENT_SOURCE_DIR}/threads
)

set(CORE_SOURCES
  CoreManager.cpp
  bindings/impl/RuntimeBindingManager.cpp
  interop/impl/InteropManager.cpp
  memory/impl/GCCollector.cpp
  memory/impl/GCDashboard.cpp
  memory/impl/GCIntegration.cpp
  memory/impl/MemoryManager.cpp
  scheduler/impl/SchedulerManager.cpp
  threads/impl/ThreadManager.cpp
)

set(CORE_HEADERS
  CoreManager.hpp
  bindings/RuntimeBindingManager.hpp
  interop/InteropManager.hpp
  memory/GCAllocator.hpp
  memory/GCCollector.hpp
  memory/GCDashboard.hpp
  memory/GCFinalizer.hpp
  memory/GCGeneration.hpp
  memory/GCIntegration.hpp
  memory/GCLogger.hpp
  memory/GCMutator.hpp
  memory/MemoryManager.hpp
  scheduler/SchedulerManager.hpp
  threads/ThreadManager.hpp
)

add_library(LynxCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCES} ${CORE_HEADERS})

llvm_map_components_to_libnames(LLVM_LIBS core executionengine)

target_link_libraries(LynxCore PRIVATE 
  ${LLVM_LIBS}
  ${GC_LIB} 
  LynxLogger 
  Threads::Threads
)

message(STATUS "Lynx Core module configured successfully")
