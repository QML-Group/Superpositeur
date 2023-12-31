cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(Superpositeur C CXX)

if(TARGET superpositeur OR TARGET superpositeur-cli)
    return()
endif()


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


#=============================================================================#
# Dependencies                                                                #
#=============================================================================#

include(FetchContent)

# None for now, except Google test in the test/ directory.

#=============================================================================#
# Superpositeur CMake library                                                 #
#=============================================================================#

add_library(superpositeur
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/API.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/Circuit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/CircuitInstruction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/DefaultOperations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/GivensRotation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/Matrix.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/MatrixSparseVectorMultiplication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/cli/SuperpositeurCLISession.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/MixedState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/QuantumOperation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/superpositeur/SparseVectorSort.cpp"
)

target_include_directories(superpositeur
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

# set(TBB_TEST OFF)
# set(TBBMALLOC_BUILD OFF)
# FetchContent_Declare(
#   tbb-project
# GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
# GIT_TAG be2fb93722eb973a495c1d5b3661d8768d58d51b
# )
# FetchContent_MakeAvailable(tbb-project)
# 
# target_link_libraries(superpositeur PUBLIC tbb)

#=============================================================================#
# Superpositeur CLI executable                                                #
#=============================================================================#

add_executable("superpositeur-cli" "${CMAKE_CURRENT_SOURCE_DIR}/bin/SuperpositeurCLI.cpp")
target_link_libraries(superpositeur-cli superpositeur)

#=============================================================================#
# Compiler options                                                            #
#=============================================================================#

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(superpositeur PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -Wno-error=restrict
        $<$<CONFIG:RELEASE>:-fomit-frame-pointer -flto>
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(superpositeur PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -Wno-error=unused-but-set-variable
        -Wno-error=unused-function
        -Wno-error=unused-local-typedef
        $<$<CONFIG:RELEASE>:-fomit-frame-pointer -flto>
    )
elseif(MSVC)
    target_compile_options(superpositeur PRIVATE
        /MP /EHsc /bigobj
    )
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

option(
    SUPERPOSITEUR_CPU_COMPATIBILITY_MODE
    "Produce binaries that would work on any CPU, instead of just the one of the machine on which the code is built"
    OFF
)

if(NOT SUPERPOSITEUR_CPU_COMPATIBILITY_MODE)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(superpositeur PUBLIC
            -march=native
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
        target_compile_options(superpositeur PUBLIC
            -ipo -xHost -no-prec-div
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(superpositeur PUBLIC
            -march=native
        )
    endif()
endif()

#=============================================================================#
# Instrumentation                                                             #
#=============================================================================#

option(
    SUPERPOSITEUR_INSTRUMENTATION
    "Output some information about runtimes for debugging performance (WIP)"
    OFF
)

if(SUPERPOSITEUR_INSTRUMENTATION)
    target_compile_definitions(superpositeur PUBLIC SUPERPOSITEUR_INSTRU)
endif()

#=============================================================================#
# Address sanitizer                                                           #
#=============================================================================#

if(ASAN_ENABLED)
    if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")

        # /MTd is needed to link clang_rt.asan-i386.lib statically
        # Otherwise the path to clang_rt.asan-i386.dll should be provided
        add_compile_options(/fsanitize=address /MTd)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

#=============================================================================#
# WASM build                                                                  #
#=============================================================================#

option(
    SUPERPOSITEUR_WASM
    "Build for web browsers supporting WebAssembly"
    OFF
)

if(SUPERPOSITEUR_WASM)
    add_definitions(-DSUPERPOSITEUR_WASM)
    
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".wasm")
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".wasm")
    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
endif()

#=============================================================================#
# Testing                                                                     #
#=============================================================================#

option(
    SUPERPOSITEUR_BUILD_TESTS
    "Whether the tests should be built and added to `make test`"
    OFF
)

if(SUPERPOSITEUR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()