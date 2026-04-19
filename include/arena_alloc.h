#include <cstddef>
#include <vector>
#include <memory>

#include "block.h"

#define STATIC_MEMORY_SIZE 4096

// Arena Allocator
class ArenaAlloc {
  public:
    
    // Partially Dynamic Allocation
    // static constexpr unsigned num_static_blocks = 10;
    // ArenaAlloc();

    // Static Allocation
    // ArenaAlloc();

    // Dynamic Allocation
    
    ArenaAlloc();
    ~ArenaAlloc();

    /**
    * @param size number of objects of type T to allocate
    * @return pointer to memory
    * allocates additional blocks if necessary, maintains allignment of T
    * size allocated must be smaller than block size
    */
    template <typename T>
    T* alloc(size_t size, size_t a = alignof(T)){
        void *temp;
        const size_t type_size = sizeof(T);
        const size_t alloc_size = size * type_size; // in bytes

        // check if allocating more than block size, and valid size
        // this does not allow 0 bytes to be requested
        if (size <= 0){
            return nullptr;
        }

        // the allocation request is valid
        
        // maintain alignment
        temp = std::align(a, type_size, block_ptr, remaining_bytes);

        // subtract bytes skipped from remaining bytes
        remaining_bytes -= static_cast<char *>(temp) - static_cast<char *>(block_ptr);
        block_ptr = temp; // update block_ptr

        // not enough room for allocation in current block
        if (remaining_bytes < alloc_size) {
            alloc_block(); // this updates remaining_bytes and block_ptr
        }

        // save current block_ptr for result
        temp = block_ptr;
        block_ptr = static_cast<char*>(block_ptr) + alloc_size;
        remaining_bytes -= alloc_size;
        
        // launder pointer
        return std::launder(reinterpret_cast<T*>(temp));
    }
    
    // returns current block's blocksize, 0 on no blocks
    size_t get_block_size();

  private:
    void alloc_block();
    
    void alloc_block_custom(size_t alloc_size);
        
    std::vector<std::unique_ptr<IBlock>> d_blocks;
    uint8_t s_block[STATIC_MEMORY_SIZE];

    int next_dblock_size;
    
    size_t remaining_bytes;
    void* block_ptr;

};