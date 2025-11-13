set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(LynxConstants INTERFACE)

target_include_directories(LynxConstants INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
