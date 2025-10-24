# --- Configurable paths ---
set(SKIA_ROOT "${CMAKE_SOURCE_DIR}/external/skia")
set(SKIA_OUT_DIR "${SKIA_ROOT}/out/Static")
set(SKIA_DONE_FILE "${SKIA_OUT_DIR}/.skia_built")

# --- Step 1: Sync Skia dependencies and generate Ninja files ---
add_custom_command(
    OUTPUT ${SKIA_OUT_DIR}/build.ninja
    COMMAND python3 ${SKIA_ROOT}/tools/git-sync-deps
    COMMAND ${SKIA_ROOT}/bin/gn gen ${SKIA_OUT_DIR} --args='is_official_build=true is_component_build=false skia_use_harfbuzz=false extra_cflags=["-Wno-psabi"]'
    WORKING_DIRECTORY ${SKIA_ROOT}
    COMMENT "Step 1: Running Skia git-sync-deps and gn gen..."
)

# --- Step 2: Build Skia with ninja if not already built ---
add_custom_command(
    OUTPUT ${SKIA_DONE_FILE}
    COMMAND ninja -C ${SKIA_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${SKIA_DONE_FILE}
    DEPENDS ${SKIA_OUT_DIR}/build.ninja
    WORKING_DIRECTORY ${SKIA_ROOT}
    COMMENT "Step 2: Building Skia with ninja..."
)

add_custom_target(build_skia ALL
    DEPENDS ${SKIA_DONE_FILE}
)

# --- Step 3: Include FindSkia module ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Skia REQUIRED)

# --- Debug info ---
message(STATUS "SKIA_DIR: ${SKIA_ROOT}")
message(STATUS "SKIA_FOUND: ${SKIA_FOUND}")
message(STATUS "SKIA_INCLUDE_DIRS: ${SKIA_INCLUDE_DIRS}")
message(STATUS "SKIA_LIBRARIES: ${SKIA_LIBRARIES}")

# --- Step 4: Your own library using Skia ---
add_library(LynxUI STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/skia_wrapper.cpp
)

add_dependencies(LynxUI build_skia)
target_include_directories(LynxUI PRIVATE ${SKIA_INCLUDE_DIRS})
target_link_libraries(LynxUI PRIVATE ${SKIA_LIBRARIES} ${SKIA_SYSTEM_LIBS})
