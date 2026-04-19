#include "arena_alloc.h"
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    // initialize gtest framework
    ::testing::InitGoogleTest(&argc, argv);
  
    // run all tests
    return RUN_ALL_TESTS();
}