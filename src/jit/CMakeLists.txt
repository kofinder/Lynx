set(JIT_LIB_HEADER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/IJIT.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/MCJITEngine.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ORCJITEngine.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/JITFactory.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/JITConstants.hpp 
)

set(JIT_LIB_IMMPL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/MCJITEngine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/ORCJITEngine.cpp
)

add_library(LynxJIT STATIC ${JIT_LIB_HEADER_SOURCES} ${JIT_LIB_IMMPL_SOURCES})

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

target_link_libraries(LynxJIT PRIVATE LynxLogger LynxCore ${LLVM_LIBS})

target_include_directories(LynxJIT PUBLIC ${LIBRARY_INCLUDES})
