/**
 * Arena Allocator
 * Allocates large slab of memory upfront, hand out pieces of it by moving a pointer forward
 * (only).
 *      - Manages objects of any size
 *      - Can not free individual objects, only the whole arena
 *
 * Compiler builds AST -> CFG (?) -> IR, and don't need the previous phase once it is over.
 * 
 * Implementation Notes:
 * 1. Define a constant block size
 * 2. Implement a Block struct representing a single chunk of memory
 *    Will hold ptr to block, and how many bytes remain.
 * 3. Implement Arena class which maintains a list of blocks and an allocate method
 *    Allocate method returns the pointer to current block or allocates a new block and returns a
 *    new pointer to it.
 * 4. Extra - cache locality implementation? think about it
 * 5. We will implement a static 10 block option followed by additional dynamic memory allocation
 *    if necessary.
 */

#include <cstddef>
#include <vector>
#include <memory>

#include "block.h"

using SmallBlock = StaticBlock<BLOCK_SIZE_SMALL>;
using MediumBlock = StaticBlock<BLOCK_SIZE_MEDIUM>;
using LargeBlock = StaticBlock<BLOCK_SIZE_LARGE>;

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

    template <typename T>
    T *alloc(size_t size, size_t a = alignof(T)); // alignof returns alignment of type

    // returns current block's blocksize, 0 on no blocks
    size_t get_block_size();
    
  private:
    void alloc_block(size_t alloc_size);
        
    // Block* static_blocks[num_static_blocks];
    std::vector<std::unique_ptr<IBlock>> _blocks;

    size_t remaining_bytes;
    void* block_ptr;

};