#include "arena_alloc.h"
#include <new>
#include <limits.h>

ArenaAlloc::ArenaAlloc() {
    block_ptr = nullptr;
    remaining_bytes = 0;
}

ArenaAlloc::~ArenaAlloc(){}

// allow for allocation of oversized block
// assumes (alloc_size + 1023) < INT_MAX
void ArenaAlloc::alloc_block(const size_t alloc_size) {
    
    if (alloc_size > INT_MAX - 1023) {
        /// @todo add error and log using utility class
        // error
    }

    // allocation size in KB, round up integer devision
    const size_t kb_required = ((alloc_size + 1023) / 1024);

    /**
     * coudln't this be a problem if the stack is filled? how can I deal with this?
     * a stack filling is a Fatal Error, so can't deal after the fact - right?
     */
    
    // allocate new block
    if (kb_required <= BLOCK_SIZE_SMALL) {
        _blocks.emplace_back(std::make_unique<SmallBlock>());
    } else if (kb_required <= BLOCK_SIZE_MEDIUM) {
        _blocks.emplace_back(std::make_unique<MediumBlock>());
    } else if (kb_required <= BLOCK_SIZE_LARGE) {
        _blocks.emplace_back(std::make_unique<LargeBlock>());
    } else {
        // fallback to DynamicBlock
        _blocks.emplace_back(std::make_unique<DynamicBlock>(kb_required));
    }
    
    // update block pointer and remaining bytes
    block_ptr = _blocks.back()->get_data();
    remaining_bytes = _blocks.back()->get_block_size();
}

/**
 * @param size number of objects of type T to allocate
 * @return pointer to memory
 * allocates additional blocks if necessary, maintains allignment of T
 * size allocated must be smaller than block size
 */
template <typename T>
T* ArenaAlloc::alloc(size_t size, size_t a){
    void *temp;
    const size_t type_size = sizeof(T);
    const size_t alloc_size = size * type_size; // in bytes

    // check if allocating more than block size, and valid size
    // this does not allow 0 bytes to be requested
    if (size <= 0){
        return nullptr;
    }

    // handle case of first block
    if (block_ptr == nullptr) {
        alloc_block(alloc_size);
    }

    // the allocation request is valid
    
    // maintain alignment
    temp = std::align(a, type_size, block_ptr, remaining_bytes);

    // subtract bytes skipped from remaining bytes
    remaining_bytes -= static_cast<char *>(temp) - static_cast<char *>(block_ptr);
    block_ptr = temp; // update block_ptr

    // not enough room for allocation in current block
    if (remaining_bytes < alloc_size) {
        alloc_block(alloc_size); // this updates remaining_bytes and block_ptr
    }

    // save current block_ptr for result
    temp = block_ptr;
    block_ptr = static_cast<char*>(block_ptr) + alloc_size;
    remaining_bytes -= alloc_size;
    
    // launder pointer
    return std::launder(reinterpret_cast<T*>(temp));
}

size_t ArenaAlloc::get_block_size() {
    if (_blocks.empty()) {
        return 0;
    }
    return _blocks.back()->get_block_size();
}