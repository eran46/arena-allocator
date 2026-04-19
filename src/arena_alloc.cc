#include "arena_alloc.h"
#include <limits.h>

ArenaAlloc::ArenaAlloc() {
    block_ptr = s_block;
    remaining_bytes = STATIC_MEMORY_SIZE;
    next_dblock_size = STATIC_MEMORY_SIZE * 2;
}

ArenaAlloc::~ArenaAlloc(){}

void ArenaAlloc::drop() {
    // free all dynamic blocks
    d_blocks.clear();

    // reset to start of internal array
    block_ptr = s_block;
    remaining_bytes = STATIC_MEMORY_SIZE;
}

void ArenaAlloc::alloc_block() {
    
    // allocation size in KB
    const size_t kb_alloc_size = next_dblock_size / 1024;

    d_blocks.emplace_back(std::make_unique<DynamicBlock>(kb_alloc_size));

    // update block pointer and remaining bytes
    block_ptr = d_blocks.back()->get_data();
    remaining_bytes = d_blocks.back()->get_block_size();
    
    next_dblock_size *= 2;
}

// dynamically allocate a custom-sized block
// assumes (alloc_size + 1023) < INT_MAX
void ArenaAlloc::alloc_block_custom(const size_t alloc_size) {
    
    if (alloc_size > INT_MAX - 1023) {
        /// @todo add error and log using utility class
        // error
    }

    // allocation size in KB, round up integer devision
    const size_t kb_alloc_size = ((alloc_size + 1023) / 1024);

    d_blocks.emplace_back(std::make_unique<DynamicBlock>(kb_alloc_size));

    // update block pointer and remaining bytes
    block_ptr = d_blocks.back()->get_data();
    remaining_bytes = d_blocks.back()->get_block_size();
}

size_t ArenaAlloc::get_block_size() {
    if (d_blocks.empty()) {
        return 0;
    }
    return d_blocks.back()->get_block_size();
}