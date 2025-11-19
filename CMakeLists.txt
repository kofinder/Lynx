cmake_minimum_required(VERSION 3.26)
project(Lynx VERSION 1.0.0 LANGUAGES C CXX)

# ===============================
# C++ Standard & Compiler Settings
# ===============================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ===============================
# Project Directories & Paths
# ===============================
set(LIBRARY_NAME Library)
set(LYNX_EXECUTABLE Lynx)
set(LIBRARY_INCLUDES "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/configured_files/include")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# ===============================
# Project Version
# ===============================
include(Version)
get_lynx_version("${PROJECT_SOURCE_DIR}/include/version.hpp")
message(STATUS "📦 Lynx Version: ${PROJECT_VERSION}")
message(STATUS "📚 Doc Tag: ${LYNX_DOC_REPO_TAG}")

# ===============================
# Options
# ===============================
option(ENABLE_TESTING "Enable Unit Testing" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
option(ENABLE_WARNINGS "Enable warnings" ON)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_SANITIZER_ADDRESS "Enable Address Sanitizer" OFF)
option(ENABLE_SANITIZER_UNDEFINED "Enable Undefined Behavior Sanitizer" OFF)
option(ENABLE_UI "Enable UI DSL support" OFF)

# ===============================
# External Dependencies & Submodules
# ===============================
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_BUILD_STATIC ON)

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

# ===============================
# Optional Integrations
# ===============================
include(FindSkia)
include(Docs)


# ===============================
# Clang-Tidy
# ===============================

if(ENABLE_CLANG_TIDY)
    include(ClangTidy)
    enable_clang_tidy_for_target(${LYNX_EXECUTABLE})
endif()

# ===============================
# Link Time Optimization (LTO)
# ===============================

if(CMAKE_BUILD_TYPE STREQUAL "Release") 
    include(LTO) 
    target_enable_lto(${LYNX_EXECUTABLE} TRUE) 
endif()

# ===============================
# Sanitizers
# ===============================
if(ENABLE_SANITIZER_ADDRESS OR ENABLE_SANITIZER_UNDEFINED)
    include(Sanitizers)
    add_sanitizer_flags()
endif()

# ===============================
# Compiler Warnings
# ===============================
if(ENABLE_WARNINGS)
    include(Warnings)
    target_set_warnings(${LYNX_EXECUTABLE} ${ENABLE_WARNINGS} ${ENABLE_WARNINGS_AS_ERRORS})
endif()

# ===============================
# UI Support
# ===============================
if(ENABLE_UI)
    # target_compile_definitions(${LYNX_EXECUTABLE} PRIVATE ENABLE_UI=1)
    # target_link_libraries(${LYNX_EXECUTABLE} ui_parser)
else()
    # target_compile_definitions(${LYNX_EXECUTABLE} PRIVATE ENABLE_UI=0)
endif()
