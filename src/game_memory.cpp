
namespace memory {

    inline u8 alignAdjustment(void *address, u8 alignment)
    {
        u8 adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & (alignment - 1));
        
        if (adjustment == alignment) return 0;

        return adjustment;
    }

    inline u8 alignAdjustmentWithHdr(void *address, u8 alignment, u8 hdr_size)
    {
        u8 adjustment = alignAdjustment(address, alignment);
        u8 needed_space = hdr_size;
        if (adjustment < needed_space) {
            needed_space -= adjustment;
            adjustment += adjustment * (needed_space / alignment);

            if (needed_space % alignment > 0) adjustment += alignment;
        }

        return adjustment;
    }

    u8 *alignForward(void *memory, u8 alignment)
    {
        const uintptr_t addr = reinterpret_cast<uintptr_t>(memory);
        const uintptr_t addr_aligned = (addr + alignment - 1) & ~(alignment - 1);
        
        u8 *memory_aligned = reinterpret_cast<u8 *>(addr_aligned);
        // if no alignment occured, shift up the full align bytes so we always have a room to store the shift
        if (memory_aligned == memory) memory_aligned += alignment;
        
        return memory_aligned;
    }


    inline void *add(void *ptr, uint32_t bytes)
	{
		return reinterpret_cast<void *>(reinterpret_cast<char *>(ptr) + bytes);
	}

    inline void *sub(void *ptr, uint32_t bytes)
	{
		return reinterpret_cast<void *>(reinterpret_cast<char *>(ptr) - bytes);
	}
}


struct Allocator
{
    Allocator(size_t size, void *start): size{size}, start{start}, used_memory{0}, num_allocations{0} {}

    virtual void *allocate(size_t size_in_bytes, u8 alignment = 16) = 0;
    virtual void deallocate(void *ptr) = 0;

    virtual ~Allocator() {
        assert(num_allocations == 0 && used_memory == 0);
        start = nullptr;
        size = 0;
    }

    void *start;
    size_t size;
    size_t used_memory;
    size_t num_allocations;
};

struct ProxyAllocator : public Allocator
{
    ProxyAllocator(Allocator &allocator);
    
    void *allocate(size_t size_in_bytes, u8 alignment) override;
    void deallocate(void *ptr) override;
    
    ProxyAllocator(const ProxyAllocator &) = delete;
    ProxyAllocator &operator=(const ProxyAllocator &) = delete;

private:
    Allocator &allocator;
};

ProxyAllocator::ProxyAllocator(Allocator &allocator): 
    Allocator(allocator.size, allocator.start), allocator{allocator} {}


void *ProxyAllocator::allocate(size_t size_in_bytes, u8 alignment)
{
    assert(size != 0);
    num_allocations++;
    size_t mem = allocator.used_memory;
    void *ptr = allocator.allocate(size_in_bytes, alignment);
    used_memory += allocator.used_memory - mem; 

    return ptr;
}

void ProxyAllocator::deallocate(void *ptr)
{
    num_allocations--;
    size_t mem = allocator.used_memory;
    allocator.deallocate(ptr);
    used_memory -= mem - allocator.used_memory;
}

struct LinearAllocator : public Allocator
{
    LinearAllocator(size_t size, void *start);
    ~LinearAllocator();
    
    LinearAllocator(const LinearAllocator &) = delete;
    LinearAllocator &operator=(const LinearAllocator &) = delete;

    void *allocate(size_t size_in_bytes, u8 alignment) override;
    void deallocate(void *ptr) override;
    void clear();

 private:
    void *current_address;
};

LinearAllocator::LinearAllocator(size_t size, void *start): Allocator(size, start), current_address{start} 
{
    assert(size > 0);
}

LinearAllocator::~LinearAllocator() 
{
    current_address = nullptr;
}

void *LinearAllocator::allocate(size_t size_in_bytes, u8 alignment)
{
    assert(size != 0);
    u8 adjustment = memory::alignAdjustment(current_address, alignment);
    if (used_memory + adjustment + size_in_bytes > size) return nullptr;
    
    u8 *aligned_address = memory::alignForward(current_address, alignment);
    current_address = reinterpret_cast<void *>(aligned_address + size_in_bytes);
    used_memory += size_in_bytes + adjustment;
    num_allocations++;

    return aligned_address;
}

void LinearAllocator::deallocate(void *ptr)
{
    assert(false && "Use clear() instead");
}

void LinearAllocator::clear()
{
    num_allocations = 0;
    used_memory = 0;
    current_address = start;
}

struct StackAllocator : public Allocator
{
    StackAllocator(size_t size, void *start);
    ~StackAllocator();
    
    StackAllocator(const StackAllocator &) = delete;
    StackAllocator &operator=(const StackAllocator &) = delete;

    void *allocate(size_t size_in_bytes, u8 alignment) override;
    void deallocate(void *ptr) override;
    void clear();

 private:
    struct Hdr
    {
        u8 adjustment;
    };

    void *current_address;
};

StackAllocator::StackAllocator(size_t size, void *start): Allocator(size, start), current_address{start} 
{
    assert(size > 0);
}

StackAllocator::~StackAllocator() 
{
    current_address = nullptr;
}

void *StackAllocator::allocate(size_t size_in_bytes, u8 alignment)
{
    assert(size_in_bytes != 0);

    u8 adjustment = memory::alignAdjustmentWithHdr(current_address, alignment, sizeof(Hdr));
    if (used_memory + adjustment + size_in_bytes > size) return nullptr;

    void *aligned_address = memory::add(current_address, adjustment);

    Hdr *header = reinterpret_cast<Hdr *>(memory::sub(aligned_address, sizeof(Hdr)));
    header->adjustment = adjustment;

    current_address = memory::add(aligned_address, size_in_bytes);
    used_memory += size_in_bytes + adjustment;
    num_allocations++;

    return aligned_address;
}

void StackAllocator::deallocate(void *ptr)
{
    Hdr *header = reinterpret_cast<Hdr *>(memory::sub(ptr, sizeof(Hdr)));
    used_memory -= reinterpret_cast<uintptr_t>(current_address) - reinterpret_cast<uintptr_t>(ptr) + header->adjustment;
    current_address = memory::sub(ptr, header->adjustment);

    num_allocations--;
}

/************** Allocation new/delete ***************************
################################################################
*/

template <typename T>
T *alloc(Allocator *allocator)
{
    return new (allocator->allocate(sizeof(T), alignof(T))) T;
}

template <typename T>
void dealloc(Allocator *allocator, T *object)
{
    object->~T();
    allocator->deallocate(object);
}

