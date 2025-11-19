# ClangTidy.cmake
# Controls enabling/disabling clang-tidy via a CMake option.
function(enable_clang_tidy_for_target target_name)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)

    if(ENABLE_CLANG_TIDY)
        if(NOT CLANG_TIDY_EXE)
            message(FATAL_ERROR "ENABLE_CLANG_TIDY is ON but clang-tidy was not found!")
        else()
            message(STATUS "✅ Clang-Tidy enabled: ${CLANG_TIDY_EXE}")
        endif()
    endif()

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY_EXE)
        set_target_properties(${target_name} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-p=${CMAKE_BINARY_DIR};--extra-arg=-std=c++23")
    endif()
endfunction()




# option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
# function(enable_clang_tidy_for_target target_name)
#     if(ENABLE_CLANG_TIDY)
#         find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)

#         if(NOT CLANG_TIDY_EXECUTABLE)
#             message(FATAL_ERROR "ENABLE_CLANG_TIDY=ON but clang-tidy not found")
#         endif()

#         set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-p=${CMAKE_BINARY_DIR};--extra-arg=-std=c++23")

#         set_target_properties(${target_name} PROPERTIES
#             CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE};--format-style=file"
#         )

#         message(STATUS "Clang-Tidy ENABLED for target: ${target_name}")
#     else()
#         message(STATUS "Clang-Tidy DISABLED for target: ${target_name}")
#     endif()
# endfunction()


