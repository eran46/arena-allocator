#include <gtest/gtest.h>
#include "arena_alloc.h"

class ArenaAllocTest : public testing::Test {
  protected:
    ArenaAllocTest() {
        _arena = std::make_unique<ArenaAlloc>();  
    }

    std::unique_ptr<ArenaAlloc> _arena;
    ArenaAlloc arena;
};

// alocation tests

// allocation
TEST_F(ArenaAllocTest, alloc) {
    uint8_t* ptr = arena.alloc<uint8_t>(1);
    ASSERT_NE(ptr, nullptr);
}

// allocate in a small block
TEST_F(ArenaAllocTest, alloc_small_block) {
    // allocate small block
    uint8_t* ptr = arena.alloc<uint8_t>(1);

    // second allocation same block
    uint8_t *ptr1 = arena.alloc<uint8_t>(1);

    ASSERT_EQ(ptr1 - ptr, 1);

    uint8_t *ptr3 = arena.alloc<uint8_t>(BLOCK_SIZE_SMALL - 2); // third allocation, fill current small block
    ASSERT_EQ(ptr3 - ptr, BLOCK_SIZE_SMALL);
}

// allocate full small block
TEST_F(ArenaAllocTest, alloc_small_block_full) {
    uint8_t *ptr = arena.alloc<uint8_t>(BLOCK_SIZE_SMALL * 1024);
    ASSERT_EQ(arena.get_block_size(), BLOCK_SIZE_SMALL);
}

// allocate medium block
TEST_F(ArenaAllocTest, alloc_medium_block) {
    uint8_t *ptr = arena.alloc<uint8_t>(BLOCK_SIZE_SMALL + 8);
    EXPECT_EQ(ptr, nullptr);
}

// allocate a small than a medium
TEST_F(ArenaAllocTest, alloc_small_to_medium) {
    uint8_t* ptr = arena.alloc<uint8_t>(BLOCK_SIZE_SMALL / 2);
    
    size_t initial_block_size = arena.get_block_size(); // initial small block size

    uint8_t *ptr4 = arena.alloc<uint8_t>(BLOCK_SIZE_SMALL * 2); // allocates new medium block
    
    ASSERT_NE(initial_block_size, arena.get_block_size());
}

// allocate large block
TEST_F(ArenaAllocTest, alloc_large_block) {
    uint8_t *ptr = arena.alloc<uint8_t>(BLOCK_SIZE_MEDIUM + 1);
    EXPECT_EQ(ptr, nullptr);
}

// allocate dynamic block
TEST_F(ArenaAllocTest, alloc_dynamic_block) {
    uint8_t *ptr = arena.alloc<uint8_t>(BLOCK_SIZE_LARGE + 1);
    EXPECT_EQ(ptr, nullptr);
}

// memory

// dynamic block freed on arena deleted
TEST_F(ArenaAllocTest, dynamic_block_freed) {
    uint8_t *ptr_small = _arena->alloc<uint8_t>(BLOCK_SIZE_SMALL);
    uint8_t *ptr_medium = _arena->alloc<uint8_t>(BLOCK_SIZE_MEDIUM);
    uint8_t *ptr_large = _arena->alloc<uint8_t>(BLOCK_SIZE_LARGE);
    uint8_t *ptr_dyn = _arena->alloc<uint8_t>(BLOCK_SIZE_LARGE + 1);

    // will fail on address sanitizer if dynamic block is not freed
    _arena.reset();
}

// alignment tests

// no alignment
TEST_F(ArenaAllocTest, small_block_alignment_1byte) {
    uint8_t *ptr_byte1 = arena.alloc<uint8_t>(1);
    
    uint8_t *ptr_byte2 = arena.alloc<uint8_t>(1); // should be adjacent
    
    ASSERT_EQ(ptr_byte2 - ptr_byte1, 1);
}

// alignment to 4 bytes
TEST_F(ArenaAllocTest, small_block_alignment_4byte) {
    uint8_t *ptr_byte1 = arena.alloc<uint8_t>(1);
    uint32_t *ptr_4byte1 = arena.alloc<uint32_t>(1); // should skip 3 bytes

    ASSERT_EQ(reinterpret_cast<uint8_t*>(ptr_4byte1) - ptr_byte1, 4);
}

// alignment to 8 bytes
TEST_F(ArenaAllocTest, small_block_alignment_8byte) {
    uint8_t *ptr_byte1 = arena.alloc<uint8_t>(1);
    uint64_t *ptr_8byte1 = arena.alloc<uint64_t>(1); // should skip 7 bytes

    ASSERT_EQ(reinterpret_cast<uint8_t *>(ptr_8byte1) - ptr_byte1, 8);
}

// uint8_t[8] uint64_t - no alignment
TEST_F(ArenaAllocTest, small_block_alignment_arr_8byte) {
    uint8_t *arr_8bytes = arena.alloc<uint8_t>(8); // byte array 8 elements
    uint64_t *ptr_8byte1 = arena.alloc<uint64_t>(1); // 8 bytes
    
    ASSERT_EQ(reinterpret_cast<uint8_t *>(ptr_8byte1) - arr_8bytes, 8);
}

