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
        message(STATUS "✅ Enabling clang-tidy for target ${target_name}: ${CLANG_TIDY_EXE}")
        set(CLANG_TIDY_CMD ${CLANG_TIDY_EXE} -p=${CMAKE_BINARY_DIR} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy --extra-arg=-std=c++23)
        set_target_properties(${target_name} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_CMD}")
    endif()

endfunction()