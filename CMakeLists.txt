cmake_minimum_required(VERSION 3.10)
project(c-ai-optimizer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find OpenMP (required for optimized builds)
find_package(OpenMP REQUIRED)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Option to build optimized version
option(BUILD_OPTIMIZED "Build optimized version" OFF)

# Include directories
include_directories(include)

# Source files
set(LIB_SOURCES
    src/matrix.c
    src/vector.c
    src/stats.c
    src/utils.c
)

# Test sources
set(TEST_SOURCES
    tests/test_runner.c
    tests/test_matrix.c
    tests/test_vector.c
    tests/test_stats.c
    tests/test_matrix_comprehensive.c
)

# Use optimized sources if enabled
if(BUILD_OPTIMIZED)
    set(SOURCES "")
    foreach(src ${LIB_SOURCES})
        string(REPLACE "src/" "src_optimized/" opt_src ${src})
        if(EXISTS "${CMAKE_SOURCE_DIR}/${opt_src}")
            list(APPEND SOURCES ${opt_src})
        else()
            list(APPEND SOURCES ${src})
        endif()
    endforeach()

    # Aggressive optimization flags for optimized build
    set(OPTIMIZED_FLAGS -O3 -march=native -mavx -ffast-math -funroll-loops)

    # Main executable - optimized
    add_executable(demo_optimized ${SOURCES} src/main.c)
    target_link_libraries(demo_optimized m)
    target_compile_definitions(demo_optimized PRIVATE OPTIMIZED_BUILD)
    target_compile_options(demo_optimized PRIVATE ${OPTIMIZED_FLAGS})
    if(OpenMP_C_FOUND)
        target_link_libraries(demo_optimized OpenMP::OpenMP_C)
    endif()

    # Test executable - optimized
    add_executable(test_optimized ${SOURCES} ${TEST_SOURCES})
    target_link_libraries(test_optimized m)
    target_compile_definitions(test_optimized PRIVATE OPTIMIZED_BUILD)
    target_compile_options(test_optimized PRIVATE ${OPTIMIZED_FLAGS})
    if(OpenMP_C_FOUND)
        target_link_libraries(test_optimized OpenMP::OpenMP_C)
    endif()
else()
    # Normal version also uses -O3 for fair comparison
    # This way we compare AI optimizations (SIMD, cache blocking) vs human code
    set(NORMAL_FLAGS -O3 -march=native)

    # Main executable - normal
    add_executable(demo ${LIB_SOURCES} src/main.c)
    target_link_libraries(demo m)
    target_compile_options(demo PRIVATE ${NORMAL_FLAGS})

    # Test executable - normal
    add_executable(test_runner ${LIB_SOURCES} ${TEST_SOURCES})
    target_link_libraries(test_runner m)
    target_compile_options(test_runner PRIVATE ${NORMAL_FLAGS})
endif()

# Custom target to build both versions
add_custom_target(build_both
    COMMAND ${CMAKE_COMMAND} -DBUILD_OPTIMIZED=OFF -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}/normal
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/normal
    COMMAND ${CMAKE_COMMAND} -DBUILD_OPTIMIZED=ON -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}/optimized
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/optimized
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
