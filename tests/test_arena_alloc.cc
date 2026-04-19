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

// allocate internal block
TEST_F(ArenaAllocTest, alloc_sblock) {
  uint8_t *ptr_byte1 = arena.alloc<uint8_t>(1);
  ASSERT_NE(ptr_byte1, nullptr);
}

// allocate dynamic block
TEST_F(ArenaAllocTest, alloc_dblock) {
    // fill static internal array
    uint8_t *ptr_byte1 = arena.alloc<uint8_t>(STATIC_MEMORY_SIZE);
    // alocate dynamic block
    uint8_t *ptr_byte2 = arena.alloc<uint8_t>(STATIC_MEMORY_SIZE);

    // check memory not contiguous
    ASSERT_NE(ptr_byte1 + STATIC_MEMORY_SIZE, ptr_byte2);
}

// memory

// dynamic block freed on arena deleted
TEST_F(ArenaAllocTest, alloc_dblock_check_drop) {
    // fill static internal array
    uint8_t *ptr_byte1 = arena.alloc<uint8_t>(STATIC_MEMORY_SIZE);
    
    // custom allocation of dynamic block
    uint8_t *ptr_byte2 = arena.alloc<uint8_t>(10 * STATIC_MEMORY_SIZE);

    uint8_t *ptr_byte3 = arena.alloc<uint8_t>(STATIC_MEMORY_SIZE);

    arena.drop();
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

