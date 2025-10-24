

set(LIB_MODULE_LINKER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Linker.hpp)

set(LIB_MODULE_LINKER_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/impl/Linker.cpp)

add_library(LynxLinker STATIC ${LIB_MODULE_LINKER_SOURCES} ${LIB_MODULE_LINKER_HEADER})

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

target_link_libraries(LynxLinker PRIVATE LynxLogger ${LLVM_LIBS})

target_include_directories(LynxLinker PUBLIC ${LIBRARY_INCLUDES})



