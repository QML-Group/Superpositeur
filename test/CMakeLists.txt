include(FetchContent)

message(STATUS "Fetching googletest")
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "f8d7d77c06936315286eb55f8de22cd23c188571"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")

add_executable(superpositeur_test
    "${CMAKE_CURRENT_SOURCE_DIR}/BitSetTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GivensRotationTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MatrixSparseVectorMultiplicationTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/IntegrationTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MixedStateTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MatrixTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/OperandPermutationTest.cpp"
)

target_link_libraries(superpositeur_test
    PRIVATE superpositeur
    PRIVATE GTest::gmock
    PRIVATE GTest::gtest_main
)

gtest_discover_tests(superpositeur_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)