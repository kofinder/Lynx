add_library(LynxRuntime SHARED
  src/runtime_types.h
  src/ctor_runtime.h
  src/ctor_runtime.cpp
  src/file_runtime.h
  src/file_runtime.c
  src/collection_runtime.h
  src/collection_runtime.cpp
)

target_include_directories(LynxRuntime PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(LynxRuntime PUBLIC LynxCollection)