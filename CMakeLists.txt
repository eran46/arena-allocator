cmake_minimum_required(VERSION 3.14)
project(arena_alloc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Use C++17 or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Fetch GoogleTest from GitHub
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP FORWARD
)
# For Windows/Linux compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

SET(ARENA_CCFILES
    src/arena_alloc.cc
    src/gtest.cc
    )

# 3. Add your actual application/library code
add_library(arenalib ${ARENA_CCFILES})
target_include_directories(arenalib PUBLIC include)

# ---- GTESTS -----

SET(ARENA_GTEST_CCFILES
    tests/test_arena_alloc.cc
    )

# 4. Create the Test Executable
enable_testing()
add_executable(run_tests ${ARENA_GTEST_CCFILES})
target_link_libraries(run_tests GTest::gtest_main arenalib)

# 5. Integrate with VS Code / CTest
include(GoogleTest)
gtest_discover_tests(run_tests)