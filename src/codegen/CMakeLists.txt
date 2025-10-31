

set(LIB_MODULE_CODE_GEN_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/IRGenerator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/IRPlanner.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TopologicalSort.hpp
)

set(LIB_MODULE_CODE_GEN_HEADER 
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/IRGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/IRPlanner.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/TopologicalSort.cpp
)

add_library(LynxCodegen STATIC ${LIB_MODULE_CODE_GEN_SOURCES} ${LIB_MODULE_CODE_GEN_HEADER})

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

target_link_libraries(LynxCodegen PRIVATE 
  LynxLogger 
  LynxAst 
  LynxLang
  LynxExceptions
  LynxProgramConfig
  Boost::system 
  Boost::filesystem
  Boost::program_options
  ${LLVM_LIBS}
)

target_include_directories(LynxCodegen PUBLIC ${LIBRARY_INCLUDES})



