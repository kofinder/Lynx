find_package(Threads REQUIRED)
find_library(GC_LIB NAMES gc REQUIRED)

message(STATUS "GC_LIB: ${GC_LIB}")
message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")

set(LYNX_LIB_MEMORY_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/memory/MemoryManager.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/memory/MemoryManager.hpp
)

set(LYNX_LIB_THREAD_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/threads/ThreadManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/threads/ThreadManager.hpp
)

set(LYNX_LIB_INTEROP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/interop/InteropManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interop/InteropManager.hpp
)

set(LYNX_LIB_SCHEDULER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/scheduler/SchedulerManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/scheduler/SchedulerManager.hpp
)

set(LYNX_LIB_BINDING_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings/RuntimeBindingManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings/RuntimeBindingManager.hpp
)

add_library(LynxCore STATIC 
  ${LYNX_LIB_MEMORY_SOURCES} 
  ${LYNX_LIB_THREAD_SOURCES}
  ${LYNX_LIB_INTEROP_SOURCES}
  ${LYNX_LIB_SCHEDULER_SOURCES}
  ${LYNX_LIB_BINDING_SOURCES}
)

llvm_map_components_to_libnames(LLVM_LIBS
  core
  mcjit
  executionengine
  native
  nativecodegen
  orcjit
  interpreter
  support
  irreader
  codegen
  transformutils
  scalaropts
)

target_link_libraries(LynxCore PRIVATE 
  LynxLogger 
  ${LLVM_LIBS}
  ${GC_LIB} 
  Threads::Threads
)

target_include_directories(LynxCore PUBLIC ${LIBRARY_INCLUDES})