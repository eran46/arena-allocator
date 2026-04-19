#include <gtest/gtest.h>
#include "block.h"


/// @todo stale gtests - rewrite

// // 1. Test if the compile-time math is correct
// TEST(DynamicBlockTest, BlockSizeCalculation) {
//     EXPECT_EQ(DynamicBlock<1>::block_size, 1024);
//     EXPECT_EQ(DynamicBlock<4>::block_size, 4096);
//     EXPECT_EQ(DynamicBlock<64>::block_size, 65536);
// }

// // 2. Test if memory is actually allocated and accessible
// TEST(DynamicBlockTest, AllocationAndAccess) {
//     DynamicBlock<4> block;
    
//     // Ensure the pointer is not null
//     ASSERT_NE(block.mem, nullptr);

//     // Verify we can write to the start and end of the block without crashing
//     block.mem[0] = 0xAA;
//     block.mem[block.block_size - 1] = 0xBB;

//     EXPECT_EQ(block.mem[0], 0xAA);
//     EXPECT_EQ(block.mem[block.block_size - 1], 0xBB);
// }

// // 3. Test Move Constructor (Ownership transfer)
// TEST(DynamicBlockTest, MoveConstructor) {
//     DynamicBlock<4> original;
//     uint8_t* original_ptr = original.mem;

//     // Move 'original' into 'moved_to'
//     DynamicBlock<4> moved_to(std::move(original));

//     // The new object should have the original pointer
//     EXPECT_EQ(moved_to.mem, original_ptr);
    
//     // The old object should now be null (based on our Move implementation)
//     EXPECT_EQ(original.mem, nullptr);
// }

// // 4. Test Move Assignment
// TEST(DynamicBlockTest, MoveAssignment) {
//     DynamicBlock<4> b1;
//     DynamicBlock<4> b2;
//     uint8_t* ptr1 = b1.mem;

//     b2 = std::move(b1);

//     EXPECT_EQ(b2.mem, ptr1);
//     EXPECT_EQ(b1.mem, nullptr);
// }

// // 5. Test that it works with different template sizes in the same scope
// TEST(DynamicBlockTest, MultipleSizes) {
//     DynamicBlock<4> small;
//     DynamicBlock<1024> large; // 1MB

//     EXPECT_EQ(small.block_size, 4096);
//     EXPECT_EQ(large.block_size, 1024 * 1024);
    
//     ASSERT_NE(small.mem, nullptr);
//     ASSERT_NE(large.mem, nullptr);
// }