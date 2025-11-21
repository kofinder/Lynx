file(GLOB_RECURSE CLI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/CLI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/commands/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/builders/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/factories/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/*.hpp"
)

file(GLOB_RECURSE CLI_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/CLI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/commands/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/builders/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/factories/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/*.hpp"
)

add_library(LynxCLI STATIC ${CLI_SOURCES} ${CLI_HEADERS})

target_include_directories(LynxCLI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/builders
    ${CMAKE_CURRENT_SOURCE_DIR}/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${LIBRARY_INCLUDES}
)

target_link_libraries(LynxCLI PUBLIC
    LynxLogger
    LynxProgramConfig
)

message(STATUS "✅ LynxCLI module configured successfully! ")
