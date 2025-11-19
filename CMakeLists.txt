cmake_minimum_required(VERSION 3.26)

project(Lynx VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_COMPILER clang)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBRARY_NAME Library)
set(LYNX_EXECUTABLE Lynx)

set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

include(Version)
get_lynx_version("${PROJECT_SOURCE_DIR}/include/version.hpp")
message(STATUS "📦 Lynx Version: ${PROJECT_VERSION}")
message(STATUS "📚 Doc Tag: ${LYNX_DOC_REPO_TAG}")


option(ENABLE_TESTING "Enable a Unit Testing Build" ON)
option(ENABLE_WARNINGS "Enable warnings" OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Enable warnings as errors" ON)
option(ENABLE_SANITIZER_ADDRESS "Enable Sanitizers Address" ON)
option(ENABLE_SANITIZER_UNDEFINED "Enable Sanitizer Undefined" ON)
option(ENABLE_UI "Enable UI DSL support" OFF)

set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_BUILD_STATIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(configured)
add_subdirectory(external)
add_subdirectory(src)


include(AddGitSubmodule)
add_git_submodule(external/json)
add_git_submodule(external/catch2)
add_git_submodule(external/cxxopts)
add_git_submodule(external/yaml-cpp)
add_git_submodule(external/spdlog)
add_git_submodule(external/fmt)
add_git_submodule(external/skia)

include(FindSkia)
include(Docs)
include(LTO)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_enable_lto(Lynx TRUE)
endif()


include(Sanitizers)
if(ENABLE_SANITIZER_ADDRESS OR ENABLE_SANITIZER_UNDEFINED)
    add_sanitizer_flags()
endif()


if(ENABLE_WARNINGS)
    include(Warnings)
    target_set_warnings(${LYNX_EXECUTABLE} ${ENABLE_WARNINGS} ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(ENABLE_UI)
    # target_compile_definitions(mylang PRIVATE ENABLE_UI=1)
    # target_link_libraries(mylang ui_parser)
else()
    # target_compile_definitions(mylang PRIVATE ENABLE_UI=0)
endif()