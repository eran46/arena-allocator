#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define BLOCK_SIZE_SMALL 4 // 4 KB
#define BLOCK_SIZE_MEDIUM 64 // 64 KB
#define BLOCK_SIZE_LARGE 1024 // 1 MB

// Generic Interface Block
struct IBlock {
    IBlock(){}
    // virtual - compiler calls derrived class versions first
    virtual ~IBlock() = default; // = defaullt - compiler handles implementation
    virtual uint8_t *get_data() = 0; // = 0 - force derrived class to implement
    virtual size_t get_block_size() = 0;

    // disable copying
    IBlock(const IBlock& other) = delete; // no copy constructor
    IBlock& operator=(const IBlock& other) = delete; // no copy assign
};

// Static allocation Block
template <size_t kb_size> struct StaticBlock : public IBlock {
    uint8_t *get_data() override {
        return mem;
    }
    size_t get_block_size() override {
        return block_size;
    }
    
    static constexpr size_t block_size = 1024 * kb_size; // bytes
    
    // allocate the block on stack
    uint8_t mem[block_size];
};

/// @todo inherit from IBlock
// Dymamic allocation Block
struct DynamicBlock : public IBlock {
    // allocate block memory on construction
    DynamicBlock(size_t kb_size) {
        block_size = kb_size * 1024;
        mem = static_cast<uint8_t*>(malloc(block_size));
    }

    // free block memory on destruction
    ~DynamicBlock(){
        free(mem);
    }

    uint8_t *get_data() override {
        return mem;
    }
    size_t get_block_size() override {
        return block_size;
    }

    // disable copying
    DynamicBlock(const DynamicBlock& other) = delete; // no copy constructor
    DynamicBlock& operator=(const DynamicBlock& other) = delete; // no copy assign

    // enable moving
    DynamicBlock(DynamicBlock&& other) noexcept : mem(other.mem) { // move constructor
        other.mem = nullptr;
    }
    DynamicBlock& operator=(DynamicBlock&& other){ // move assign
        if (this != &other){
            free(mem);
            mem = other.mem;
            other.mem = nullptr;
        }
        return *this;
    }

    size_t block_size;
    uint8_t* mem;
};