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
get_lynx_version("${PROJECT_SOURCE_DIR}/configured/version.hpp")
message(STATUS "📦 Lynx Version: ${PROJECT_VERSION}")
message(STATUS "📚 Doc Tag: ${LYNX_DOC_REPO_TAG}")

# ===============================
# Options
# ===============================
option(ENABLE_TESTING "Enable Unit Testing" OFF)
option(ENABLE_LTO "Enable LTO optimizations" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" ON)
option(ENABLE_WARNINGS "Enable warnings" OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
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

# ===============================
# Optional Integrations
# ===============================
include(Docs)

# ===============================
# Clang-Tidy
# ===============================
if(ENABLE_CLANG_TIDY)
    include(ClangTidy)
    enable_clang_tidy_for_target(${LYNX_EXECUTABLE})
    enable_clang_tidy_for_target(LynxLogger)
    # enable_clang_tidy_for_target(LynxAnalyzer)
endif()

# ===============================
# Link Time Optimization (LTO)
# ===============================
include(LTO) 
if(ENABLE_LTO)
    set(CMAKE_LINKER /usr/local/llvm-21/bin/clang++)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
endif()

# ===============================
# Sanitizers
# ===============================
include(Sanitizers)
if(ENABLE_SANITIZER_ADDRESS OR ENABLE_SANITIZER_UNDEFINED)
    add_sanitizer_flags()
endif()

# ===============================
# Compiler Warnings
# ===============================
include(Warnings)
if(ENABLE_WARNINGS)
    target_set_warnings(${LYNX_EXECUTABLE} ${ENABLE_WARNINGS} ${ENABLE_WARNINGS_AS_ERRORS})
endif()
