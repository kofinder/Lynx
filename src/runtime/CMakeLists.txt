set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Define the static library
add_library(LynxRuntime STATIC ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})

# Include directories
target_include_directories(LynxRuntime
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/collections
    ${CMAKE_CURRENT_SOURCE_DIR}/collections/array/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/collections/array
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/directory
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/path
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/util
)

# Link dependencies
target_link_libraries(LynxRuntime PRIVATE
  LynxLogger
  LynxContext
  LynxConstants
  LynxAst
  LynxTypes
)

set_target_properties(LynxRuntime PROPERTIES POSITION_INDEPENDENT_CODE ON)

message(STATUS "✅ LynxRuntime module configured successfully!")
