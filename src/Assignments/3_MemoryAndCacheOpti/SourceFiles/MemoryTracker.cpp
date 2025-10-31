
#include "../HeaderFiles/MemoryTracker.h"

// Define static members
size_t MemoryTracker::totalAllocated = 0;
size_t MemoryTracker::peakAllocated = 0;
size_t MemoryTracker::allocCount = 0;
size_t MemoryTracker::deallocCount = 0;
size_t MemoryTracker::simAllocated = 0;

// Override global new/delete
void* operator new(std::size_t size)
{
    MemoryTracker::totalAllocated += size;
    MemoryTracker::allocCount++;

    if (MemoryTracker::totalAllocated > MemoryTracker::peakAllocated)
        MemoryTracker::peakAllocated = MemoryTracker::totalAllocated;

    return std::malloc(size);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    MemoryTracker::totalAllocated -= size;
    MemoryTracker::deallocCount++;
    std::free(ptr);
}

void* operator new[](std::size_t size)
{
    MemoryTracker::totalAllocated += size;
    MemoryTracker::allocCount++;

    if (MemoryTracker::totalAllocated > MemoryTracker::peakAllocated)
        MemoryTracker::peakAllocated = MemoryTracker::totalAllocated;

    return std::malloc(size);
}

void operator delete[](void* ptr, std::size_t size) noexcept
{
    MemoryTracker::totalAllocated -= size;
    MemoryTracker::deallocCount++;
    std::free(ptr);
}