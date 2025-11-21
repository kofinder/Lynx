# ---------------------------------------------------------
# Define source directories
# ---------------------------------------------------------
set(LTO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(STRATEGY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/strategies")
set(PASSES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/passes")
set(PASSES_IMPL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/passes/impl")

include_directories(
  ${LTO_INCLUDE_DIR}
  ${STRATEGY_INCLUDE_DIR}
  ${PASSES_INCLUDE_DIR}
  ${PASSES_IMPL_DIR}
)

# ---------------------------------------------------------
# Collect all pass source files
# ---------------------------------------------------------
file(GLOB PASS_IMPL_SOURCES "${PASSES_IMPL_DIR}/*.cpp")

# ---------------------------------------------------------
# Create a shared library for all passes
# ---------------------------------------------------------
add_library(LynxLTO STATIC ${PASS_IMPL_SOURCES})

# Output name without lib prefix
set_target_properties(LynxLTO PROPERTIES PREFIX "" OUTPUT_NAME "LynxLTOPasses")
set_target_properties(LynxLTO PROPERTIES POSITION_INDEPENDENT_CODE ON)

llvm_map_components_to_libnames(LLVM_LIBS
  Core
  Analysis
  Support
  Passes
)

target_link_libraries(LynxLTO PRIVATE ${LLVM_LIBS})

message(STATUS "✅ LynxLTO module configured successfully! ")




